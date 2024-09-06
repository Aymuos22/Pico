// lexer.cpp
#include "lexer.h"
#include <cctype>
#include <unordered_map>

// Constructor for the Lexer class
Lexer::Lexer(const std::string& filename, const std::string& text)
    : filename(filename), text(text), pos(0) {
    current_char = text[pos];
}

// Advance the `pos` and update the `current_char`
void Lexer::advance() {
    pos++;
    current_char = pos < text.size() ? text[pos] : '\0';
}

// Skip whitespace characters
void Lexer::skip_whitespace() {
    while (isspace(current_char)) {
        advance();
    }
}

// Make a number token (integer and float support)
Token Lexer::make_number() {
    std::string num_str;
    bool has_dot = false;

    while (isdigit(current_char) || current_char == '.') {
        if (current_char == '.') {
            if (has_dot) break; // Only one dot allowed
            has_dot = true;
        }
        num_str += current_char;
        advance();
    }

    return Token(TokenType::NUMBER, num_str, pos);
}

// Make an identifier or keyword token
Token Lexer::make_identifier() {
    std::string id_str;

    while (isalnum(current_char) || current_char == '_') {
        id_str += current_char;
        advance();
    }

    // Recognize specific keywords
    static std::unordered_map<std::string, TokenType> keywords = {
        {"print", TokenType::PRINT}  // Recognize 'print' as PRINT token
    };

    // Check if identifier is a keyword, otherwise it's an identifier
    TokenType type = keywords.count(id_str) ? keywords[id_str] : TokenType::IDENTIFIER;

    return Token(type, id_str, pos);
}

// Make a string token
Token Lexer::make_string() {
    std::string str_val;
    advance(); // Skip the opening quote

    while (current_char != '"' && current_char != '\0') {
        str_val += current_char;
        advance();
    }

    advance(); // Skip the closing quote
    return Token(TokenType::STRING, str_val, pos);
}

// Make a token for operators or unknown characters
Token Lexer::make_operator() {
    char op_char = current_char;
    advance();

    switch (op_char) {
        case '+': return Token(TokenType::PLUS, "+", pos);
        case '-': return Token(TokenType::MINUS, "-", pos);
        case '*': return Token(TokenType::MULTIPLY, "*", pos);
        case '/': return Token(TokenType::DIVIDE, "/", pos);
        case '(': return Token(TokenType::LPAREN, "(", pos);
        case ')': return Token(TokenType::RPAREN, ")", pos);
        default:  return Token(TokenType::UNKNOWN, std::string(1, op_char), pos);
    }
}

// Generate tokens from the input text
std::pair<std::vector<Token>, Error> Lexer::make_tokens() {
    std::vector<Token> tokens;

    while (current_char != '\0') {
        if (isspace(current_char)) {
            skip_whitespace();
        } else if (isdigit(current_char)) {
            tokens.push_back(make_number());
        } else if (isalpha(current_char)) {
            tokens.push_back(make_identifier());
        } else if (current_char == '"') {
            tokens.push_back(make_string());
        } else {
            tokens.push_back(make_operator());
        }
    }

    return { tokens, Error() };  // No errors
}
