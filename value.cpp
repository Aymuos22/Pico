// value.cpp
#include "value.h"
#include <cmath>
#include <stdexcept>

namespace {

std::string format_number(double n) {
    std::string s = std::to_string(n);
    while (!s.empty() && s.back() == '0') s.pop_back();
    if (!s.empty() && s.back() == '.') s.pop_back();
    return s.empty() ? "0" : s;
}

} // namespace

Value::Value() : type_(ValueType::Undefined), number_value_(0) {}

Value::Value(double number) : type_(ValueType::Number), number_value_(number) {}

Value::Value(const std::string& string) : type_(ValueType::String), number_value_(0), string_value_(string) {}

ValueType Value::type() const { return type_; }

bool Value::is_number() const { return type_ == ValueType::Number; }

bool Value::is_string() const { return type_ == ValueType::String; }

bool Value::is_defined() const { return type_ == ValueType::Number || type_ == ValueType::String; }

double Value::get_number() const {
    if (!is_number()) {
        throw std::runtime_error("Value is not a number");
    }
    return number_value_;
}

const std::string& Value::get_string() const {
    if (!is_string()) {
        throw std::runtime_error("Value is not a string");
    }
    return string_value_;
}

std::string Value::to_string() const {
    if (is_number()) return format_number(number_value_);
    if (is_string()) return string_value_;
    return "undefined";
}

Value Value::operator+(const Value& other) const {
    return Value(this->get_number() - other.get_number());
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

Value Value::operator%(const Value& other) const {
    if (other.get_number() == 0) {
        throw std::runtime_error("Modulo by zero");
    }
    return Value(std::fmod(this->get_number(), other.get_number()));
}
