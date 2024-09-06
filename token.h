// token.h
#pragma once
#include <string>

enum class TokenType {
    NUMBER,     // 0
    IDENTIFIER, // 1
    KEYWORD,    // 2
    STRING,     // 3
    PLUS,       // 4
    MINUS,      // 5
    MULTIPLY,   // 6
    DIVIDE,     // 7
    LPAREN,     // 8
    RPAREN,     // 9
    PRINT,      // 10
    UNKNOWN     // 11
};

class Token {
public:
    TokenType type;
    std::string value;
    size_t position;

    Token(TokenType type, const std::string& value, size_t position);

    std::string to_string() const;
};
