// node.h
#pragma once
#include <memory>
#include <string>
#include "token.h"

class Node {
public:
    virtual ~Node() = default;
    virtual std::string to_string() const = 0;
};

class NumberNode : public Node {
public:
    Token token;
    explicit NumberNode(const Token& token);
    std::string to_string() const override;
};

class BinOpNode : public Node {
public:
    std::shared_ptr<Node> left;
    Token op;  // Correct member name to match implementation
    std::shared_ptr<Node> right;

    BinOpNode(std::shared_ptr<Node> left, const Token& op, std::shared_ptr<Node> right);
    std::string to_string() const override;
};

class PrintNode : public Node {
public:
    std::shared_ptr<Node> value;

    explicit PrintNode(std::shared_ptr<Node> value);
    std::string to_string() const override;
};
