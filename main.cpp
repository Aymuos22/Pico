// main.cpp
#include <iostream>
#include <string>
#include <fstream>
#include "run.h"

int main(int argc, char* argv[]) {
    std::string filename;
    std::string code;

    // Check if filename is provided as a command-line argument
    if (argc > 1) {
        filename = argv[1];
    } else {
        // Default to 'test.txt' if no argument is given
        filename = "test.txt";
    }

    // Open the file and read the code
    std::ifstream file(filename);
    if (file.is_open()) {
        code.assign((std::istreambuf_iterator<char>(file)),
                    (std::istreambuf_iterator<char>()));
        file.close();
    } else {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return 1; // Exit with an error code
    }

    // Run the interpreter with the provided code
    std::pair<Value, Error> result_error = run(filename, code);
    Value result = result_error.first;  // Extract result from pair
    Error error = result_error.second;  // Extract error from pair

    // Check for errors and display the results
    if (!error.is_empty()) {
        std::cout << "Error: " << error.as_string() << std::endl;
    } else {
        std::cout << "Result: " << result.to_string() << std::endl;
    }

    return 0;
}
