// node.cpp
#include "node.h"

// Implement NumberNode
NumberNode::NumberNode(const Token& token) : token(token) {}

std::string NumberNode::to_string() const {
    return "NumberNode(" + token.value + ")";
}

// Implement BinOpNode
BinOpNode::BinOpNode(std::shared_ptr<Node> left, const Token& op, std::shared_ptr<Node> right)
    : left(left), op(op), right(right) {}

std::string BinOpNode::to_string() const {
    return "BinOpNode(" + left->to_string() + ", " + op.value + ", " + right->to_string() + ")";
}

// Implement PrintNode
PrintNode::PrintNode(std::shared_ptr<Node> value) : value(value) {}

std::string PrintNode::to_string() const {
    return "PrintNode(" + value->to_string() + ")";
}
