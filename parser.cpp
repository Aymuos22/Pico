// parser.cpp
#include "parser.h"
#include <iostream>  // For std::cerr and std::endl

// Constructor for the Parser class
Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens), pos(0), current_token(Token(TokenType::UNKNOWN, "", 0)) {
    if (!tokens.empty()) {
        current_token = tokens[pos];
    }
}

// Advance to the next token
void Parser::advance() {
    pos++;
    if (pos < tokens.size()) {
        current_token = tokens[pos];
    }
}

// Parse a print statement
std::shared_ptr<Node> Parser::print_statement() {
    if (current_token.type == TokenType::PRINT) {
        advance();  // Skip the 'PRINT' token

        // Expect an expression after 'PRINT'
        auto expr = expression();  // Parse the expression to print

        if (!expr) {
            std::cerr << "Error: Expected an expression after 'print'" << std::endl;
            return nullptr;
        }

        return std::make_shared<PrintNode>(expr);  // Return the print node
    } else {
        std::cerr << "Error: 'print_statement' called without PRINT token" << std::endl;
        return nullptr;
    }
}

// Parse a factor (numbers, expressions in parentheses)
std::shared_ptr<Node> Parser::factor() {
    Token token = current_token;

    if (token.type == TokenType::NUMBER) {
        advance();
        return std::make_shared<NumberNode>(token);
    } 
    else if (token.type == TokenType::LPAREN) {
        advance();  // Skip '('
        auto expr = expression();  // Parse the expression inside parentheses
        if (current_token.type == TokenType::RPAREN) {
            advance();  // Skip ')'
            return expr;
        } else {
            std::cerr << "Error: Expected closing parenthesis" << std::endl;
            return nullptr;
        }
    }

    std::cerr << "Error: Unexpected factor of type " << static_cast<int>(token.type) 
              << " with value '" << token.value << "'" << std::endl;  // Improved error handling
    return nullptr;  // Handle invalid factor error
}

// Parse terms (handles multiplication and division)
std::shared_ptr<Node> Parser::term() {
    auto left = factor();  // Start with a factor

    while (current_token.type == TokenType::MULTIPLY || current_token.type == TokenType::DIVIDE) {
        Token op = current_token;
        advance();  // Skip operator
        auto right = factor();  // Parse the right-hand side factor
        if (!right) {
            std::cerr << "Error: Expected a factor after operator" << std::endl;
            return nullptr;
        }
        left = std::make_shared<BinOpNode>(left, op, right);  // Build binary operation node
    }

    return left;
}

// Parse expressions (handles addition and subtraction)
std::shared_ptr<Node> Parser::expression() {
    auto left = term();  // Start with a term

    while (current_token.type == TokenType::PLUS || current_token.type == TokenType::MINUS) {
        Token op = current_token;
        advance();  // Skip operator
        auto right = term();  // Parse the right-hand side term
        if (!right) {
            std::cerr << "Error: Expected a term after operator" << std::endl;
            return nullptr;
        }
        left = std::make_shared<BinOpNode>(left, op, right);  // Build binary operation node
    }

    return left;
}

// Parse statements (distinguishes between statements and expressions)
std::shared_ptr<Node> Parser::parse_statement() {
    if (current_token.type == TokenType::PRINT) {
        return print_statement();  // Handle print statement
    }

    // If no specific statement, fallback to parsing as an expression
    return expression();
}

// Main parse function that starts parsing from the statement level
std::pair<std::shared_ptr<Node>, Error> Parser::parse() {
    if (tokens.empty()) {
        return { nullptr, Error("No tokens to parse", 0) };  // Handle empty input
    }

    auto result = parse_statement();  // Start parsing from the statement level

    // Ensure all tokens were consumed; if not, handle unexpected tokens
    if (pos < tokens.size()) {
        return { nullptr, Error("Unexpected token", tokens[pos].position) };
    }

    return std::make_pair(result, Error());  // Return the parsed result with no errors
}
