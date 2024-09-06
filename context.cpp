// context.cpp
#include "context.h"

// Constructor for the Context class
Context::Context(const std::string& display_name, std::shared_ptr<Context> parent, size_t parent_entry_pos)
    : display_name(display_name), parent(parent), parent_entry_pos(parent_entry_pos), symbol_table(std::make_shared<SymbolTable>()) {}

// Sets the symbol table for the current context
void Context::set_symbol_table(std::shared_ptr<SymbolTable> new_symbol_table) {
    symbol_table = new_symbol_table;
}
