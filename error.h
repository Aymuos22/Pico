// error.h
#pragma once
#include <string>

class Error {
private:
    std::string message;  // Description of the error
    size_t position;      // Position in the source code where the error occurred
    bool has_error;       // Indicates if an error exists

public:
    // Constructor for initializing an error with a message and position
    Error(const std::string& message = "", size_t position = 0);

    // Checks if there is an error
    bool is_empty() const;

    // Returns a string representation of the error
    std::string as_string() const;
};
