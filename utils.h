// utils.h
#pragma once
#include <string>
#include <vector>

// Utility function to split a string by a delimiter
std::vector<std::string> split(const std::string& str, char delimiter);

// Utility function to check if a string represents a number
bool is_number(const std::string& str);

// Utility function to trim whitespace from the start and end of a string
std::string trim(const std::string& str);
