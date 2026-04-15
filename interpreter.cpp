// interpreter.cpp
#include "interpreter.h"
#include <chrono>
#include <cmath>
#include <cctype>
#include <iostream>
#include <random>
#include <string>
#include <thread>

namespace {

std::string trace_indent(Context& ctx) {
    return std::string(static_cast<size_t>(ctx.trace_depth * 2), ' ');
}

void trim_line(std::string& s) {
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) {
        s.erase(s.begin());
    }
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) {
        s.pop_back();
    }
}

Value value_from_input_line(const std::string& line) {
    std::string s = line;
    trim_line(s);
    if (s.empty()) {
        return Value(std::string(""));
    }
    try {
        size_t idx = 0;
        double d = std::stod(s, &idx);
        if (idx == s.size()) {
            return Value(d);
        }
    } catch (...) {}
    return Value(s);
}

std::mt19937& rng() {
    thread_local std::mt19937 gen(std::random_device{}());
    return gen;
}

struct TraceRestore {
    Context& ctx;
    std::ostream* saved_stream;
    int saved_depth;

    explicit TraceRestore(Context& c) : ctx(c), saved_stream(c.trace_stream), saved_depth(c.trace_depth) {
        ctx.trace_stream = &std::cout;
        ctx.trace_depth = 0;
    }

    ~TraceRestore() {
        ctx.trace_stream = saved_stream;
        ctx.trace_depth = saved_depth;
    }

    TraceRestore(const TraceRestore&) = delete;
    TraceRestore& operator=(const TraceRestore&) = delete;
};

} // namespace

std::pair<Value, Error> Interpreter::visit_NumberNode(std::shared_ptr<NumberNode> node, Context& context) {
    try {
        Value v(std::stod(node->token.value));
        if (context.trace_stream) {
            *context.trace_stream << trace_indent(context) << "num " << node->token.value << " => " << v.to_string() << "\n";
        }
        return { v, Error() };
    } catch (...) {
        return { Value(), Error("Invalid number literal", node->token.position) };
    }
}

std::pair<Value, Error> Interpreter::visit_StringNode(std::shared_ptr<StringNode> node, Context& context) {
    Value v(node->token.value);
    if (context.trace_stream) {
        *context.trace_stream << trace_indent(context) << "str \"" << node->token.value << "\"\n";
    }
    return { v, Error() };
}

std::pair<Value, Error> Interpreter::visit_VarNode(std::shared_ptr<VarNode> node, Context& context) {
    if (node->name_token.value == "that") {
        if (!context.has_that) {
            return { Value(), Error("no value for 'that' yet — run a statement that produces a value first", node->name_token.position) };
        }
        if (context.trace_stream) {
            *context.trace_stream << trace_indent(context) << "that => " << context.that_value.to_string() << "\n";
        }
        return { context.that_value, Error() };
    }

    Value v = context.symbol_table->get(node->name_token.value);
    if (!v.is_defined()) {
        return { Value(), Error("Undefined variable '" + node->name_token.value + "'", node->name_token.position) };
    }
    if (context.trace_stream) {
        *context.trace_stream << trace_indent(context) << "var " << node->name_token.value << " => " << v.to_string() << "\n";
    }
    return { v, Error() };
}

std::pair<Value, Error> Interpreter::visit_UnaryMinusNode(std::shared_ptr<UnaryMinusNode> node, Context& context) {
    const bool tracing = context.trace_stream != nullptr;
    if (tracing) context.trace_depth++;

    auto inner = visit(node->expr, context);

    if (tracing) context.trace_depth--;

    if (!inner.second.is_empty()) return inner;

    if (!inner.first.is_number()) {
        return { Value(), Error("Unary '-' requires a number", 0) };
    }

    Value out(-inner.first.get_number());
    if (context.trace_stream) {
        *context.trace_stream << trace_indent(context) << "neg => " << out.to_string() << "\n";
    }
    return { out, Error() };
}

std::pair<Value, Error> Interpreter::visit_BinOpNode(std::shared_ptr<BinOpNode> node, Context& context) {
    const bool tracing = context.trace_stream != nullptr;
    if (tracing) context.trace_depth++;

    auto left_result = visit(node->left, context);
    if (!left_result.second.is_empty()) {
        if (tracing) context.trace_depth--;
        return left_result;
    }

    auto right_result = visit(node->right, context);
    if (!right_result.second.is_empty()) {
        if (tracing) context.trace_depth--;
        return right_result;
    }

    if (tracing) context.trace_depth--;

    Value left_value = left_result.first;
    Value right_value = right_result.first;

    if (!left_value.is_number() || !right_value.is_number()) {
        return { Value(), Error("Arithmetic requires numeric operands", node->op.position) };
    }

    try {
        Value result;

        if (node->op.type == TokenType::PLUS) {
            result = left_value + right_value;
        } else if (node->op.type == TokenType::MINUS) {
            result = Value(left_value.get_number() - right_value.get_number());
        } else if (node->op.type == TokenType::MULTIPLY) {
            result = left_value / right_value;
        } else if (node->op.type == TokenType::DIVIDE) {
            if (right_value.get_number() == 0) {
                return { Value(), Error("Division by zero", node->op.position) };
            }
            result = left_value / right_value;
        } else if (node->op.type == TokenType::MODULO) {
            if (right_value.get_number() == 0) {
                return { Value(), Error("Modulo by zero", node->op.position) };
            }
            result = Value(std::fmod(left_value.get_number(), right_value.get_number()));
        } else {
            return { Value(), Error("Unknown operator", node->op.position) };
        }

        if (context.trace_stream) {
            *context.trace_stream << trace_indent(context)
                                  << left_value.to_string() << " " << node->op.value << " "
                                  << right_value.to_string() << " => " << result.to_string() << "\n";
        }

        return { result, Error() };

    } catch (const std::runtime_error& e) {
        return { Value(), Error(e.what(), node->op.position) };
    }
}

std::pair<Value, Error> Interpreter::visit_TraceNode(std::shared_ptr<TraceNode> node, Context& context) {
    TraceRestore guard(context);
    auto inner = visit(node->expr, context);
    if (!inner.second.is_empty()) {
        return inner;
    }
    if (inner.first.is_defined()) {
        context.set_that(inner.first);
    }
    return inner;
}

std::pair<Value, Error> Interpreter::visit_TimesNode(std::shared_ptr<TimesNode> node, Context& context) {
    auto count_result = visit(node->count_expr, context);
    if (!count_result.second.is_empty()) {
        return count_result;
    }
    if (!count_result.first.is_number()) {
        return { Value(), Error("times count must be a number", 0) };
    }
    double n = count_result.first.get_number();
    if (std::floor(n) != n || n < 0) {
        return { Value(), Error("times requires a non-negative integer count", 0) };
    }
    int count = static_cast<int>(n);
    Value last;
    for (int i = 0; i < count; ++i) {
        auto step = visit(node->body, context);
        if (!step.second.is_empty()) {
            return step;
        }
        last = step.first;
        if (step.first.is_defined()) {
            context.set_that(step.first);
        }
    }
    return { last, Error() };
}

std::pair<Value, Error> Interpreter::visit_ReadNode(std::shared_ptr<ReadNode> node, Context& context) {
    std::string line;
    if (!std::getline(std::cin, line)) {
        return { Value(), Error("read failed (end of input)", node->at.position) };
    }
    Value v = value_from_input_line(line);
    if (context.trace_stream) {
        *context.trace_stream << trace_indent(context) << "read => " << v.to_string() << "\n";
    }
    context.set_that(v);
    return { v, Error() };
}

std::pair<Value, Error> Interpreter::visit_SleepNode(std::shared_ptr<SleepNode> node, Context& context) {
    auto ms_result = visit(node->ms_expr, context);
    if (!ms_result.second.is_empty()) {
        return ms_result;
    }
    if (!ms_result.first.is_number()) {
        return { Value(), Error("sleep requires a numeric milliseconds argument", node->at.position) };
    }
    double ms = ms_result.first.get_number();
    if (std::floor(ms) != ms || ms < 0 || ms > 86400000) {
        return { Value(), Error("sleep requires an integer milliseconds in [0, 86400000]", node->at.position) };
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(ms)));
    if (context.trace_stream) {
        *context.trace_stream << trace_indent(context) << "sleep " << ms << "ms\n";
    }
    return { Value(), Error() };
}

std::pair<Value, Error> Interpreter::visit_PrintNode(std::shared_ptr<PrintNode> node, Context& context) {
    auto result = visit(node->value, context);
    if (!result.second.is_empty()) return result;

    context.set_that(result.first);
    std::cout << result.first.to_string() << std::endl;
    return { Value(), Error() };
}

std::pair<Value, Error> Interpreter::visit_LetNode(std::shared_ptr<LetNode> node, Context& context) {
    if (node->name == "that") {
        return { Value(), Error("'that' is reserved (read-only pronoun)", 0) };
    }

    auto result = visit(node->value, context);
    if (!result.second.is_empty()) return result;

    if (!result.first.is_defined()) {
        return { Value(), Error("let requires a defined value", 0) };
    }

    context.symbol_table->set(node->name, result.first);
    context.set_that(result.first);
    return { Value(), Error() };
}

std::pair<Value, Error> Interpreter::visit_AssignNode(std::shared_ptr<AssignNode> node, Context& context) {
    if (node->name == "that") {
        return { Value(), Error("'that' is reserved (read-only pronoun)", 0) };
    }

    auto result = visit(node->value, context);
    if (!result.second.is_empty()) return result;

    if (!result.first.is_defined()) {
        return { Value(), Error("Assignment requires a defined value", 0) };
    }

    context.symbol_table->set(node->name, result.first);
    context.set_that(result.first);
    return { Value(), Error() };
}

std::pair<Value, Error> Interpreter::visit_RandNode(std::shared_ptr<RandNode> node, Context& context) {
    auto hi_result = visit(node->hi_expr, context);
    if (!hi_result.second.is_empty()) {
        return hi_result;
    }

    if (!hi_result.first.is_number()) {
        return { Value(), Error("rand requires a numeric upper bound", node->at.position) };
    }

    double n = hi_result.first.get_number();
    if (std::floor(n) != n || n <= 0) {
        return { Value(), Error("rand requires a positive integer upper bound", node->at.position) };
    }

    int max_value = static_cast<int>(n);
    std::uniform_int_distribution<int> dist(1, max_value);
    Value out(static_cast<double>(dist(rng())));

    if (context.trace_stream) {
        *context.trace_stream << trace_indent(context)
                              << "rand " << max_value << " => " << out.to_string() << "\n";
    }

    return { out, Error() };
}

std::pair<Value, Error> Interpreter::visit_ProgramNode(std::shared_ptr<ProgramNode> node, Context& context) {
    Value last;

    for (const auto& stmt : node->statements) {
        auto step = visit(stmt, context);
        if (!step.second.is_empty()) return step;

        last = step.first;
        if (step.first.is_defined()) {
            context.set_that(step.first);
        }
    }

    return { last, Error() };
}

std::pair<Value, Error> Interpreter::visit(std::shared_ptr<Node> node, Context& context) {
    if (auto n = std::dynamic_pointer_cast<NumberNode>(node)) return visit_NumberNode(n, context);
    if (auto n = std::dynamic_pointer_cast<StringNode>(node)) return visit_StringNode(n, context);
    if (auto n = std::dynamic_pointer_cast<VarNode>(node)) return visit_VarNode(n, context);
    if (auto n = std::dynamic_pointer_cast<UnaryMinusNode>(node)) return visit_UnaryMinusNode(n, context);
    if (auto n = std::dynamic_pointer_cast<BinOpNode>(node)) return visit_BinOpNode(n, context);
    if (auto n = std::dynamic_pointer_cast<PrintNode>(node)) return visit_PrintNode(n, context);
    if (auto n = std::dynamic_pointer_cast<LetNode>(node)) return visit_LetNode(n, context);
    if (auto n = std::dynamic_pointer_cast<AssignNode>(node)) return visit_AssignNode(n, context);
    if (auto n = std::dynamic_pointer_cast<TraceNode>(node)) return visit_TraceNode(n, context);
    if (auto n = std::dynamic_pointer_cast<TimesNode>(node)) return visit_TimesNode(n, context);
    if (auto n = std::dynamic_pointer_cast<ReadNode>(node)) return visit_ReadNode(n, context);
    if (auto n = std::dynamic_pointer_cast<RandNode>(node)) return visit_RandNode(n, context);
    if (auto n = std::dynamic_pointer_cast<SleepNode>(node)) return visit_SleepNode(n, context);
    if (auto n = std::dynamic_pointer_cast<ProgramNode>(node)) return visit_ProgramNode(n, context);

    return { Value(), Error("Internal error: unknown node type", 0) };
}
