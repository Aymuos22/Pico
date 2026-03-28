// run.cpp
#include "run.h"
#include <memory>
#include <vector>
#include "lexer.h"
#include "node.h"
#include "parser.h"
#include "interpreter.h"
#include "context.h"
#include "symbol_table.h"
#include <iostream>

std::pair<Value, Error> run(const std::string& filename, const std::string& code, const RunOptions& options) {
    Lexer lexer(filename, code);
    std::pair<std::vector<Token>, Error> lex_result = lexer.make_tokens();
    std::vector<Token> tokens = lex_result.first;
    Error lex_error = lex_result.second;

    if (!lex_error.is_empty()) {
        return { Value(), lex_error };
    }

    if (options.dump_tokens) {
        std::cout << "Tokens:" << std::endl;
        for (const auto& token : tokens) {
            std::cout << token.to_string() << std::endl;
        }
    }

    Parser parser(tokens);
    std::pair<std::shared_ptr<Node>, Error> parse_result = parser.parse();
    std::shared_ptr<Node> ast = parse_result.first;
    Error parse_error = parse_result.second;

    if (!parse_error.is_empty()) {
        return { Value(), parse_error };
    }

    Interpreter interpreter;
    Context context("<program>");
    context.symbol_table = std::make_shared<SymbolTable>();

    std::pair<Value, Error> interpret_result = interpreter.visit(ast, context);

    return interpret_result;
}