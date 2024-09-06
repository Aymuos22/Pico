// error.cpp
#include "error.h"

// Constructor for Error class
Error::Error(const std::string& message, size_t position)
    : message(message), position(position), has_error(!message.empty()) {}

// Checks if there is no error (error message is empty)
bool Error::is_empty() const {
    return !has_error;
}

// Converts the error to a string representation
std::string Error::as_string() const {
    if (has_error) {
        return "Error at position " + std::to_string(position) + ": " + message;
    }
    return "No Error";
}
