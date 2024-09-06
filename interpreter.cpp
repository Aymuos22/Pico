// interpreter.cpp
#include "interpreter.h"
#include <iostream>

// Visit a number node
std::pair<Value, Error> Interpreter::visit_NumberNode(std::shared_ptr<NumberNode> node, Context& context) {
    return { Value(std::stoi(node->token.value)), Error() };
}

// Visit a binary operation node
std::pair<Value, Error> Interpreter::visit_BinOpNode(std::shared_ptr<BinOpNode> node, Context& context) {
    std::pair<Value, Error> left_result = visit(node->left, context);
    Value left_value = left_result.first;
    Error left_error = left_result.second;

    if (!left_error.is_empty()) return { Value(), left_error };

    std::pair<Value, Error> right_result = visit(node->right, context);
    Value right_value = right_result.first;
    Error right_error = right_result.second;

    if (!right_error.is_empty()) return { Value(), right_error };

    Value result;
    if (node->op.type == TokenType::PLUS) {
        result = left_value.get_number() + right_value.get_number();
    } else if (node->op.type == TokenType::MINUS) {
        result = left_value.get_number() - right_value.get_number();
    } else if (node->op.type == TokenType::MULTIPLY) {
        result = left_value.get_number() * right_value.get_number();
    } else if (node->op.type == TokenType::DIVIDE) {
        if (right_value.get_number() == 0) {
            return { Value(), Error("Division by zero", node->op.position) };
        }
        result = left_value.get_number() / right_value.get_number();
    } else {
        return { Value(), Error("Unknown operator", node->op.position) };
    }

    return { result, Error() };
}

// Visit a print node
std::pair<Value, Error> Interpreter::visit_PrintNode(std::shared_ptr<PrintNode> node, Context& context) {
    std::pair<Value, Error> result = visit(node->value, context);
    Value value = result.first;
    Error error = result.second;

    if (!error.is_empty()) return { Value(), error };

    // Print the evaluated value
    std::cout << value.to_string() << std::endl;
    return { Value(), Error() };  // Return an empty value with no error
}

// Visit a node (main interpreter logic)
std::pair<Value, Error> Interpreter::visit(std::shared_ptr<Node> node, Context& context) {
    if (auto number_node = std::dynamic_pointer_cast<NumberNode>(node)) {
        return visit_NumberNode(number_node, context);
    }
    if (auto binop_node = std::dynamic_pointer_cast<BinOpNode>(node)) {
        return visit_BinOpNode(binop_node, context);
    }
    if (auto print_node = std::dynamic_pointer_cast<PrintNode>(node)) {
        return visit_PrintNode(print_node, context);
    }

    return { Value(), Error("No visit method for this node", 0) };
}
