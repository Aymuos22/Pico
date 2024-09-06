// run.cpp
#include "run.h"
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "context.h"
#include "symbol_table.h"
#include <iostream>

std::pair<Value, Error> run(const std::string& filename, const std::string& code) {
    // Generate tokens using the lexer
    Lexer lexer(filename, code);
    std::pair<std::vector<Token>, Error> lex_result = lexer.make_tokens();
    std::vector<Token> tokens = lex_result.first;
    Error lexError = lex_result.second;

    // Check for lexer errors
    if (!lexError.is_empty()) {
        return { Value(), lexError };
    }

    // Print tokens for debugging
    std::cout << "Tokens:" << std::endl;
    for (const auto& token : tokens) {
        std::cout << token.to_string() << std::endl;
    }

    // Parse the tokens into an AST
    Parser parser(tokens);
    std::pair<std::shared_ptr<Node>, Error> parse_result = parser.parse();
    std::shared_ptr<Node> ast = parse_result.first;
    Error parseError = parse_result.second;

    // Check for parser errors
    if (!parseError.is_empty()) {
        return { Value(), parseError };
    }

    // Interpret the AST
    Interpreter interpreter;
    Context context("<program>");
    context.symbol_table = std::make_shared<SymbolTable>();  // Initialize global symbol table
    std::pair<Value, Error> interpret_result = interpreter.visit(ast, context);
    Value result = interpret_result.first;
    Error runtimeError = interpret_result.second;

    return { result, runtimeError };
}
