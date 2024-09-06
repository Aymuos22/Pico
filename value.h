// value.h
#pragma once
#include <string>

class Value {
private:
    double number_value;  // Numeric value representation
    bool is_number_type;  // Flag to indicate if the value is a number

public:
    // Constructors
    Value();
    Value(double number);

    // Checks if the value is a number
    bool is_number() const;

    // Retrieves the numeric value
    double get_number() const;

    // Converts the value to a string representation
    std::string to_string() const;

    // Operator overloads for arithmetic operations
    Value operator+(const Value& other) const;
    Value operator-(const Value& other) const;
    Value operator*(const Value& other) const;
    Value operator/(const Value& other) const;
};
