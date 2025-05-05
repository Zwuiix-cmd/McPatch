#pragma once
#include <string>
#include <vector>
#include <conio.h>
#include <iostream>

#include "../Utils/Logger.h"

class FormMenu {
public:
    static void drawForm(const std::vector<std::string>& options, const std::vector<bool>& selected, int current) {
        system("cls");
        Logger::displaySeparator(false);
        Logger::print("&fSelect options (press &cSPACE&f to &ctoggle&f, &cENTER&f to &cconfirm&f):\n\n", false);

        for (size_t i = 0; i < options.size(); ++i) {
            std::string prefix = (int)i == current ? "&6> " : "  ";
            std::string mark = selected[i] ? "&8[&ax&r&8] &f" : "&8[&cx&r&8] &f";
            Logger::print(prefix + mark + "&f" + options[i], false);
        }

        Logger::print("", false);
        Logger::displaySeparator(false);
    }

    static std::vector<bool> apply(const std::vector<std::string>& options) {
        int current = 0;
        std::vector selected(options.size(), false);

        while (true) {
            drawForm(options, selected, current);

            int key = _getch();
            if (key == 224) key = _getch();

            switch (key) {
                case 72:
                    current = (current - 1 + options.size()) % options.size();
                break;
                case 80:
                    current = (current + 1) % options.size();
                break;
                case 32:
                    selected[current] = !selected[current];
                break;
                case 13:
                    return selected;
            }
        }
    }
};
