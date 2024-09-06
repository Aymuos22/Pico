// strings_with_arrows.h
#pragma once
#include <string>

// Function that generates a string with arrows pointing to the error location
std::string string_with_arrows(const std::string& text, size_t start, size_t end);
