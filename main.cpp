#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <windows.h>
#include <chrono>

#include "src/utils/Game.h"
#include "src/utils/Logger.h"

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

bool patchExe(const std::string& inputFile, const std::string& outputFile,
              const std::unordered_map<std::string, std::vector<int>>& patches, const std::vector<std::string> strings) {

    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile) {
        Logger::error("Failed to open the source file!");
        return false;
    }

    inFile.seekg(0, std::ios::end);
    size_t fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);

    std::vector<unsigned char> fileData((std::istreambuf_iterator<char>(inFile)),
                                        std::istreambuf_iterator<char>());
    inFile.close();

    Logger::info("Reading file... (" + std::to_string(fileSize / 1024) + " KB)");
    Logger::info("Starting patching signatures...");
    Logger::displaySeparator();

    for (const auto& [signatureStr, replacement] : patches) {
        std::vector<int> signature = parseSignature(signatureStr);
        size_t pos = findSignature(fileData, signature);
        if (pos == std::string::npos) {
            Logger::error("Signature not found: " + signatureStr);
            continue;
        }

        for (size_t i = 0; i < replacement.size(); i++) {
            if (replacement[i] != -1) {
                fileData[pos + i] = replacement[i];
            }
        }

        Logger::info("Patch applied for signature: " + signatureStr);
    }

    Logger::displaySeparator();

    auto patchEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> patchDuration = patchEnd - start;
    Logger::info("Patching complete! Patching time: " + std::to_string(patchDuration.count()) + " seconds.");

    std::ofstream outFile(outputFile, std::ios::binary);
    if (!outFile) {
        Logger::error("Failed to create the patched file!");
        return false;
    }

    Logger::info("Writing patched file...");

    auto writeStart = std::chrono::high_resolution_clock::now();
    outFile.write(reinterpret_cast<const char*>(fileData.data()), fileData.size());
    outFile.close();

    auto writeEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> writeDuration = writeEnd - writeStart;

    Logger::info("Patched file saved as " + outputFile);
    Logger::info("Write time: " + std::to_string(writeDuration.count()) + " seconds.");

    auto totalEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> totalDuration = totalEnd - start;
    Logger::info("Total process time: " + std::to_string(totalDuration.count()) + " seconds.");

    return true;
}

std::vector<int> askPatchChoices() {
    std::vector<int> choices;
    std::string response;

    Logger::info("Which patches would you like to apply? (Y/N for each):");

    Logger::print("Apply GuiScale patch? (Y/N): ");
    std::getline(std::cin, response);
    if (response == "Y" || response == "y") {
        choices.push_back(1);
    }

    Logger::print("Apply NoAimDelay patch? (Y/N): ");
    std::getline(std::cin, response);
    if (response == "Y" || response == "y") {
        choices.push_back(2);
    }

    if (choices.empty()) {
        Logger::warn("No patches selected.");
    }

    return choices;
}

int main() {
    std::unordered_map<std::string, std::vector<int>> patches = {};
    std::vector<std::string> strings = {};

    Logger::info("Starting the patching process...");
    Logger::displaySeparator();

    if (!Game::isStarted()) {
        Logger::info("Minecraft is not running, attempting to launch it...");
        Logger::displaySeparator();
        system("start minecraft://");
    }

    Logger::info("Waiting for Minecraft to start...");

    while (!Game::isStarted()) {}

    std::string path = Game::getPath();

    Logger::info("Minecraft found at: " + path);
    Logger::displaySeparator();

    Game::kill();

    std::vector<int> selectedPatches = askPatchChoices();
    Logger::displaySeparator();

    if (std::find(selectedPatches.begin(), selectedPatches.end(), 1) != selectedPatches.end()) {
        patches["00 00 ? ? 00 00 A0 40 00 00 C0 40"] = {0x00, 0x00, 0xE0};
    }
    if (std::find(selectedPatches.begin(), selectedPatches.end(), 2) != selectedPatches.end()) {
        patches["E8 ? ? ? ? 48 8B 03 48 8D 94 24 ? ? ? ? 48 8B 4B"] = {0x90, 0x90, 0x90, 0x90, 0x90};
    }

    if (selectedPatches.empty()) {
        Logger::error("No patches selected. Exiting.");
        system("pause");
        return 0;
    }

    std::string outputFile = "Minecraft.Patched.exe";
    Logger::info("Starting the patching process...");

    if (patchExe(path, path, patches, strings)) {
        Logger::info("File patched and saved as " + outputFile);
    } else {
        Logger::error("Error occurred while patching the file.");
    }

    Logger::displaySeparator();
    Logger::info("Patching process complete. You can now launch the patched file.");
    Logger::displaySeparator();

    system("start https://kodiak.best/");
    system("start https://discord.gg/sYERkksn6q");

    MessageBoxA(nullptr, "Successful!", "McPatch", MB_OK | MB_ICONINFORMATION);
    system("start minecraft://");
    return 0;
}
