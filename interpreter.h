// interpreter.h
#pragma once
#include <memory>
#include "node.h"
#include "context.h"
#include "symbol_table.h"
#include "value.h"
#include "error.h"

class Interpreter {
public:
    std::pair<Value, Error> visit(std::shared_ptr<Node> node, Context& context);
    std::pair<Value, Error> visit_NumberNode(std::shared_ptr<NumberNode> node, Context& context);
    std::pair<Value, Error> visit_BinOpNode(std::shared_ptr<BinOpNode> node, Context& context);
    std::pair<Value, Error> visit_PrintNode(std::shared_ptr<PrintNode> node, Context& context);  // Add this line
};
