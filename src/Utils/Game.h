#pragma once
#include <bemapiset.h>
#include <psapi.h>
#include <string>
#include <tlhelp32.h>
#include <windows.h>

class Game {
private:
    inline static std::string process = "Minecraft.Windows.exe";
public:
    static bool isStarted() {
        return GetProcess() != nullptr;
    }

    inline static void kill() {
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE) return;

        PROCESSENTRY32 processEntry;
        processEntry.dwSize = sizeof(PROCESSENTRY32);
        if (Process32First(hSnapshot, &processEntry)) {
            do {
                if (strcmp(processEntry.szExeFile, process.c_str()) == 0) {
                    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processEntry.th32ProcessID);
                    if (hProcess) {
                        TerminateProcess(hProcess, 0);
                        CloseHandle(hProcess);
                    }
                }
            } while (Process32Next(hSnapshot, &processEntry));
        }

        CloseHandle(hSnapshot);
    }

    inline static std::string getVersion() {
        HKEY hKey;
        const char* subKey = "Software\\Classes\\Local Settings\\Software\\Microsoft\\Windows\\CurrentVersion\\AppModel\\Repository\\Packages";
        const char* packagePrefix = "Microsoft.MinecraftUWP_";

        if (RegOpenKeyExA(HKEY_CURRENT_USER, subKey, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
            return "Unknown";

        char name[256];
        DWORD index = 0;
        DWORD nameSize = sizeof(name);

        while (RegEnumKeyExA(hKey, index++, name, &nameSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
            if (strncmp(name, packagePrefix, strlen(packagePrefix)) == 0) {
                std::string full = name;
                size_t start = full.find('_') + 1;
                size_t end = full.find('_', start);
                if (start != std::string::npos && end != std::string::npos)
                    return full.substr(start, end - start);
            }
            nameSize = sizeof(name);
        }

        RegCloseKey(hKey);
        return "Unknown";
    }

    static std::string getShortPath(size_t maxLength = 40) {
        std::string path = getPath();
        if (path.length() <= maxLength) return path;

        std::string ellipsis = "...";
        size_t charsToShow = maxLength - ellipsis.length();
        return ellipsis + path.substr(path.length() - charsToShow);
    }

    static std::string getPath() {
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snapshot == INVALID_HANDLE_VALUE) return "";

        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(PROCESSENTRY32);
        std::string processPath = "";

        if (Process32First(snapshot, &pe)) {
            do {
                if (process == pe.szExeFile) {
                    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe.th32ProcessID);
                    if (hProcess) {
                        char path[MAX_PATH];
                        if (GetModuleFileNameExA(hProcess, NULL, path, MAX_PATH)) {
                            processPath = path;
                        }
                        CloseHandle(hProcess);
                    }
                    break;
                }
            } while (Process32Next(snapshot, &pe));
        }

        CloseHandle(snapshot);
        return processPath;
    }



    static HANDLE GetProcess()
    {
        DWORD pid = 0;

        const HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32 process;
        ZeroMemory(&process, sizeof(process));
        process.dwSize = sizeof(process);

        if (Process32First(snapshot, &process))
        {
            do
            {
                if (std::string(process.szExeFile) == Game::process)
                {
                    pid = process.th32ProcessID;
                    break;
                }
            } while (Process32Next(snapshot, &process));
        }

        CloseHandle(snapshot);

        if (pid != 0)
        {
            return OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION | PROCESS_ALL_ACCESS, FALSE, pid);
        }

        return nullptr;
    }
};
