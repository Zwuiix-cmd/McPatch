#pragma once

#include <iostream>
#include <rang.hpp>
#include <string>

class Logger {
public:
    static void info(std::string t) {
        std::cout << rang::fg::cyan << "[" << rang::style::bold << "INFO" << rang::style::reset << rang::fg::cyan << "] " << rang::fg::reset << t << std::endl;
        std::cout.flush();
    }
    static void print(std::string t) {
        std::cout << rang::fg::cyan << "[" << rang::style::bold << "INFO" << rang::style::reset << rang::fg::cyan << "] " << rang::fg::reset << t;
        std::cout.flush();
    }
    static void error(std::string t) {
        std::cout << rang::fg::red << "[" << rang::style::bold << "ERROR" << rang::style::reset << rang::fg::red << "] " << t << std::endl;
        std::cout.flush();
    }
    static void warn(std::string t) {
        std::cout << rang::fg::yellow << "[" << rang::style::bold << "WARN" << rang::style::reset << rang::fg::yellow << "] " << t << std::endl;
        std::cout.flush();
    }

    static void displaySeparator() {
        std::cout << rang::fg::cyan << "=====================================" << rang::style::reset << std::endl;
    }
};