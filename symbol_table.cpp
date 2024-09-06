// symbol_table.cpp
#include "symbol_table.h"

// Constructor for SymbolTable, optionally setting a parent symbol table
SymbolTable::SymbolTable(std::shared_ptr<SymbolTable> parent)
    : parent(parent) {}

// Retrieves the value of a variable, checking parent scopes if necessary
Value SymbolTable::get(const std::string& name) {
    if (symbols.find(name) != symbols.end()) {
        return symbols[name];
    } else if (parent != nullptr) {
        return parent->get(name);
    } else {
        return Value(); // Return a default value if not found
    }
}

// Sets the value of a variable in the current symbol table
void SymbolTable::set(const std::string& name, const Value& value) {
    symbols[name] = value;
}

// Removes a variable from the current symbol table
void SymbolTable::remove(const std::string& name) {
    symbols.erase(name);
}
