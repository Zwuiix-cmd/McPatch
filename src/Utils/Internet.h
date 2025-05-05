#pragma once

#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <string>

#pragma comment(lib, "wininet.lib")

class Internet {
public:
    inline static std::string HttpGetRequest(const std::string& url) {
        DWORD bytesRead;
        char buffer[4096];
        std::string response;

        std::wstring wurl(url.begin(), url.end());

        HINTERNET hInternet = InternetOpenW(L"HTTP User Agent", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if (hInternet == nullptr) {
            return "";
        }

        HINTERNET hConnect = InternetOpenUrlW(hInternet, wurl.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (hConnect == nullptr) {
            InternetCloseHandle(hInternet);
            return "";
        }

        while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
            response.append(buffer, bytesRead);
        }

        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);

        return response;
    }
};