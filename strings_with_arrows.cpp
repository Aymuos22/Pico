// strings_with_arrows.cpp
#include "strings_with_arrows.h"

// Generates a string with arrows pointing to the error location in the text
std::string string_with_arrows(const std::string& text, size_t start, size_t end) {
    std::string result;
    size_t idx_start = start;
    size_t idx_end = end;

    // Find the start and end of the line containing the error
    while (idx_start > 0 && text[idx_start - 1] != '\n') idx_start--;
    while (idx_end < text.size() && text[idx_end] != '\n') idx_end++;

    // Extract the relevant line and create the arrow markers
    std::string line = text.substr(idx_start, idx_end - idx_start);
    result += line + "\n";
    result += std::string(start - idx_start, ' '); // Indentation for the arrow
    result += std::string(std::max<size_t>(1, end - start), '^'); // Arrow length

    return result;
}
