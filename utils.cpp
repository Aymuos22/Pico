// utils.cpp
#include "utils.h"
#include <sstream>
#include <cctype>
#include <algorithm>

// Splits a string by a specified delimiter and returns the parts as a vector
std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> parts;
    std::stringstream ss(str);
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        parts.push_back(item);
    }

    return parts;
}

// Checks if a given string can be converted to a number (integer or float)
bool is_number(const std::string& str) {
    std::istringstream iss(str);
    double val;
    iss >> val;
    return !iss.fail() && iss.eof();
}

// Trims whitespace from both ends of a string
std::string trim(const std::string& str) {
    std::string trimmed = str;
    trimmed.erase(trimmed.begin(), std::find_if(trimmed.begin(), trimmed.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    trimmed.erase(std::find_if(trimmed.rbegin(), trimmed.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), trimmed.end());
    return trimmed;
}
