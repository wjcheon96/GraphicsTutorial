#include "Common.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

std::optional<std::string> LoadTextFile(const std::string& filename) {
    std::cout << filename << '\n';
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        SPDLOG_ERROR("failed to open file: {}", filename);
        return {};
    }
    std::stringstream text;
    text << fin.rdbuf();
    return text.str();
}
