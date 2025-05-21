#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <windows.h>
#include <chrono>

#include "src/Form/FormMenu.hpp"
#include "src/SDK/SDK.hpp"
#include "src/Utils/Game.h"
#include "src/Utils/Logger.h"

void CreateConsole() {
    AllocConsole();

    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);
    freopen_s(&f, "CONIN$", "r", stdin);
}

std::vector<int> parseSignature(const std::string& signature) {
    std::vector<int> bytes;
    size_t pos = 0;
    while (pos < signature.length()) {
        if (signature[pos] == '?') {
            bytes.push_back(-1);
            pos += 2;
        } else {
            bytes.push_back(std::stoi(signature.substr(pos, 2), nullptr, 16));
            pos += 3;
        }
    }
    return bytes;
}

size_t findSignature(const std::vector<unsigned char>& data, const std::vector<int>& signature) {
    for (size_t i = 0; i <= data.size() - signature.size(); i++) {
        bool found = true;
        for (size_t j = 0; j < signature.size(); j++) {
            if (signature[j] != -1 && data[i + j] != signature[j]) {
                found = false;
                break;
            }
        }
        if (found) return i;
    }
    return std::string::npos;
}

bool patchExe(const std::string& inputFile, const std::string& outputFile, const std::string& saveOldFile,
              const std::unordered_map<std::string, std::pair<std::string, std::vector<int>>>& patches) {
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile) {
        Logger::log("File", "&cFailed to open the source file!");
        return false;
    }

    inFile.seekg(0, std::ios::end);
    size_t fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);

    std::vector<unsigned char> fileData(fileSize);
    inFile.read(reinterpret_cast<char*>(fileData.data()), fileSize);
    inFile.close();

    std::ofstream outOldFile(saveOldFile, std::ios::binary);
    outOldFile.write(reinterpret_cast<const char*>(fileData.data()), fileData.size());
    outOldFile.close();

    Logger::log("File", "Reading file... &8[&c" + std::to_string(fileSize / 1024) + "kb&8]");

    for (const auto& [key, patchData] : patches) {
        const std::string& signatureStr = patchData.first;
        const std::vector<int>& replacement = patchData.second;

        std::vector<int> signature = parseSignature(signatureStr);

        bool foundAny = false;

        if (key == "PlayScreenFix") {
            size_t searchStart = 0;
            while (searchStart < fileData.size()) {
                size_t pos = findSignature(fileData, signature);
                if (pos == std::string::npos) break;

                foundAny = true;
                for (size_t i = 0; i < replacement.size(); ++i) {
                    if (replacement[i] != -1) {
                        fileData[pos + i] = replacement[i];
                    }
                }

                searchStart = pos + 1;
            }
        } else {
            size_t pos = findSignature(fileData, signature);
            if (pos == std::string::npos) {
                Logger::log("File", "Signature &c" + key + "&r not found.");
                continue;
            }

            for (size_t i = 0; i < replacement.size(); ++i) {
                if (replacement[i] != -1) {
                    fileData[pos + i] = replacement[i];
                }
            }

            Logger::log("File", "Signature &c" + key + "&r found");
        }

        if (key == "PlayScreenFix") {
            foundAny ?
            Logger::log("File", "Signature &c" + key + "&r found") :
            Logger::log("File", "Signature &c" + key + "&r not found.");
        }
    }

    auto patchEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> patchDuration = patchEnd - start;
    Logger::log("File", "Patching complete. &8[&c" + std::to_string(patchDuration.count()) + "s&8]");

    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile) {
        Logger::log("File", "&cFailed to create the patched file!");
        return false;
    }

    auto writeStart = std::chrono::high_resolution_clock::now();
    outFile.write(reinterpret_cast<const char*>(fileData.data()), fileData.size());
    outFile.close();

    auto writeEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> writeDuration = writeEnd - writeStart;

    Logger::log("File", "File saved. &8[&c" + std::to_string(writeDuration.count()) + "s&8]");

    auto totalEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> totalDuration = totalEnd - start;
    Logger::log("File", "Process time &c" + std::to_string(totalDuration.count()) + " seconds&r.");
    Logger::displaySeparator();

    return true;
}

void applyStyles() {
    CreateConsole();
    HANDLE hConsole = GetConsoleWindow();

    SetConsoleTitleA("McPatch - Minecraft Bedrock Edition Patcher");

    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    GetCurrentConsoleFontEx(hConsole, FALSE, &cfi);

    cfi.dwFontSize.Y = 18;
    wcscpy_s(cfi.FaceName, L"Cascadia Code Bold");

    SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);

    DWORD mode;
    GetConsoleMode(hConsole, &mode);
    mode |= ENABLE_QUICK_EDIT_MODE;
    mode |= ENABLE_INSERT_MODE;
    SetConsoleMode(hConsole, mode);
}

int main() {
    applyStyles();

    Logger::displaySeparator();
    Logger::log("SDK", "&cDownload in progress...");
    SDK::parseSDK();
    SDK::parsePatches();
    Logger::displaySeparator();

    if(!Game::isStarted()) {
        Logger::log("GAME", "&7Retrieving the game path...");
        system("start minecraft://");

        while (!Game::isStarted()) {}
    }

    std::string path = Game::getPath();
    std::string shortPath = Game::getShortPath();

    Logger::log("GAME", "&rFound at &c" + shortPath);
    Game::kill();
    Logger::displaySeparator();

    auto options = SDK::getAllSigKeys();
    auto result = FormMenu::apply(options);
    auto version = Game::getVersion();

    system("cls");
    Logger::displaySeparator(false);
    Logger::log("GAME", "&rPath: &c" + shortPath, false);
    Logger::log("GAME", "&rVersion: &c" + Game::getVersion(), false);
    Logger::displaySeparator(false);

    std::unordered_map<std::string, std::pair<std::string, std::vector<int>>> patches = {};

    for (size_t i = 0; i < options.size(); ++i) {
        if (result[i]) {
            std::string key = options[i];
            patches[key] = std::make_pair(SDK::getSig(key), SDK::getPatch(key));
        }
    }

    if (patchExe(path, path, "Minecraft.Windows." + version + ".exe", patches)) {
        Logger::log("File", "The data has been written in your game.");
    } else {
        Logger::log("Error","Error occurred while patching the file.");
    }
    Logger::displaySeparator();

    Logger::print("&rPress &cENTER &rto &claunch the game&r...");
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    system("start minecraft://");

    return 0;
}
