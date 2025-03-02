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
