// parser.h
#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <memory>
#include "token.h"
#include "node.h"
#include "error.h"

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    std::pair<std::shared_ptr<Node>, Error> parse();
    std::shared_ptr<Node> parse_statement();  // Declare parse_statement here

private:
    void advance();
    std::shared_ptr<Node> print_statement();
    std::shared_ptr<Node> factor();
    std::shared_ptr<Node> term();
    std::shared_ptr<Node> expression();

    std::vector<Token> tokens;
    size_t pos;
    Token current_token;
};

#endif // PARSER_H
