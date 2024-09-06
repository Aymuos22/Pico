// context.h
#pragma once
#include <string>
#include <memory>
#include "symbol_table.h"

class Context {
public:
    std::string display_name;                    // Name of the current context
    std::shared_ptr<SymbolTable> symbol_table;   // Pointer to the symbol table used in this context
    std::shared_ptr<Context> parent;             // Pointer to the parent context
    size_t parent_entry_pos;                     // Position in the parent context (if applicable)

    // Constructor for creating a new context
    Context(const std::string& display_name, std::shared_ptr<Context> parent = nullptr, size_t parent_entry_pos = 0);

    // Sets the symbol table for this context
    void set_symbol_table(std::shared_ptr<SymbolTable> new_symbol_table);
};
