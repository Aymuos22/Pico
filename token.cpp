// token.cpp
#include "token.h"

// Implement Token constructor
Token::Token(TokenType type, const std::string& value, size_t position)
    : type(type), value(value), position(position) {}

// Implement Token's to_string method
std::string Token::to_string() const {
    return "Token(" + std::to_string(static_cast<int>(type)) + ", '" + value + "', " + std::to_string(position) + ")";
}
