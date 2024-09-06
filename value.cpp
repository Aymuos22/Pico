// value.cpp
#include "value.h"
#include <stdexcept>

// Default constructor, initializes to zero and not a number
Value::Value() : number_value(0), is_number_type(false) {}

// Constructor for numeric values
Value::Value(double number) : number_value(number), is_number_type(true) {}

// Checks if the current value is of type number
bool Value::is_number() const {
    return is_number_type;
}

// Retrieves the numeric value, throws if not a number
double Value::get_number() const {
    if (!is_number_type) {
        throw std::runtime_error("Value is not a number");
    }
    return number_value;
}

// Converts the value to a string representation
std::string Value::to_string() const {
    if (is_number_type) {
        return std::to_string(number_value);
    }
    return "Undefined"; // Placeholder for non-number types
}

// Operator overloads for arithmetic operations
Value Value::operator+(const Value& other) const {
    return Value(this->get_number() + other.get_number());
}

Value Value::operator-(const Value& other) const {
    return Value(this->get_number() - other.get_number());
}

Value Value::operator*(const Value& other) const {
    return Value(this->get_number() * other.get_number());
}

Value Value::operator/(const Value& other) const {
    if (other.get_number() == 0) {
        throw std::runtime_error("Division by zero");
    }
    return Value(this->get_number() / other.get_number());
}
