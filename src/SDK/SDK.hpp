#pragma once
#include <string>
#include <unordered_set>

#include "../Utils/Internet.h"
#include "../Utils/json.hpp"
#include "../Utils/Logger.h"

using json = nlohmann::json;

class SDK {
private:
    inline static std::unordered_map<std::string, std::string> signatures = std::unordered_map<std::string, std::string>();
    inline static std::unordered_map<std::string,  std::vector<int>> patches = std::unordered_map<std::string, std::vector<int>>();
public:
    inline static void parseSDK() {
        std::string url = "https://raw.githubusercontent.com/Zwuiix-cmd/McPatch/refs/heads/sdk/sdk.json";
        std::string jsonResponse = Internet::HttpGetRequest(url);

        Logger::log("SDK", "&cSuccessfully retrieved.");
        json parsed = json::parse(jsonResponse);

        for (auto& el : parsed.items()) {
            const std::string& key = el.key();
            const json& value = el.value();

            signatures[key] = value.get<std::string>();
        }
        Logger::log("SDK", "&c" + std::to_string(signatures.size()) + "&r&7 signatures successfully loaded.");
    }

    inline static void parsePatches() {
        std::string url = "https://raw.githubusercontent.com/Zwuiix-cmd/McPatch/refs/heads/sdk/patches.json";
        std::string jsonResponse = Internet::HttpGetRequest(url);

        json parsed = json::parse(jsonResponse);
        for (auto& el : parsed.items()) {
            const std::string& key = el.key();
            const json& value = el.value();

            std::vector<int> patchValues;
            std::string hexString = value.get<std::string>();

            std::stringstream ss(hexString);
            std::string hexValue;
            while (ss >> hexValue) {
                int intValue;
                std::stringstream converter;
                converter << std::hex << hexValue;
                converter >> intValue;
                patchValues.push_back(intValue);
            }

            patches[key] = patchValues;
        }

        Logger::log("SDK", "&c" + std::to_string(patches.size()) + "&r&7 patches successfully loaded.");

    }

    inline static std::unordered_map<std::string, std::string> getSigs() {
        return signatures;
    }

    inline static std::string getSig(const std::string& prefix) {
        auto it = signatures.find(prefix);
        if (it != signatures.end()) {
            return it->second;
        }

        return "";
    }

    inline static std::vector<int> getPatch(const std::string& prefix) {
        auto it = patches.find(prefix);
        if (it != patches.end()) {
            return it->second;
        }

        return {};
    }

    inline static std::vector<std::string> getAllSigKeys() {
        std::vector<std::string> keys;
        keys.reserve(signatures.size());
        for (const auto& pair : signatures) {
            keys.push_back(pair.first);
        }
        return keys;
    }

};
