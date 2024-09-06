// lexer.h
#pragma once
#include <vector>
#include <string>
#include "token.h"
#include "error.h"

class Lexer {
private:
    std::string filename;
    std::string text;
    size_t pos;
    char current_char;

    void advance();
    void skip_whitespace();
    Token make_number();
    Token make_identifier();
    Token make_string();
    Token make_operator();

public:
    Lexer(const std::string& filename, const std::string& text);

    std::pair<std::vector<Token>, Error> make_tokens();
};
