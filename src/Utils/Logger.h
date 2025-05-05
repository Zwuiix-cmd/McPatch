#pragma once
#include <iostream>
#include <rang.hpp>
#include <string>
#include <thread>
#include <unordered_map>

class Logger {
private:
    static std::string minecraftToAnsi(std::string text) {
        std::unordered_map<char, std::string> colorMap = {
            {'0', "\033[30m"},  // Noir
         {'1', "\033[34m"},  // Bleu
         {'2', "\033[32m"},  // Vert
         {'3', "\033[36m"},  // Cyan (corrigé ici)
         {'4', "\033[31m"},  // Rouge
         {'5', "\033[35m"},  // Magenta
         {'6', "\033[33m"},  // Jaune
         {'7', "\033[37m"},  // Gris clair (correct)
         {'8', "\033[90m"},  // Gris foncé
         {'9', "\033[91m"},  // Rouge clair
         {'a', "\033[32m"},  // Vert (pour &a)
         {'b', "\033[36m"},  // Cyan (pour &b, corrigé)
         {'c', "\033[91m"},  // Rouge clair
         {'d', "\033[35m"},  // Magenta
         {'e', "\033[93m"},  // Jaune clair
         {'f', "\033[97m"},  // Blanc
         {'r', "\033[0m"}    // Réinitialisation des couleurs
        };

        std::string result = "";
        for (size_t i = 0; i < text.length(); ++i) {
            if (text[i] == '&' && i + 1 < text.length()) {
                char colorCode = text[i + 1];
                if (colorMap.find(colorCode) != colorMap.end()) {
                    result += colorMap[colorCode];
                    ++i;
                }
            } else {
                result += text[i];
            }
        }
        return result;
    }

public:
    static void print(std::string text, bool anim = true) {
        std::cout << rang::style::reset;
        std::cout.flush();

        for (char c : minecraftToAnsi(text)) {
            std::cout << c;
            std::cout.flush();
            if(anim) std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        std::cout << std::endl;
    }

    static void log(std::string prefix, std::string t = "", bool anim = true) {
        print("&8[&r" + prefix + "&8] &r" + t, anim);
    }

    static void displaySeparator(bool anim = true) {
        std::string separator(37, '=');
        print("&r&8" + separator, anim);
    }
};
