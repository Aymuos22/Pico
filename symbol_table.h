// symbol_table.h
#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "value.h"

class SymbolTable {
private:
    std::unordered_map<std::string, Value> symbols; // Map to store variables and their values
    std::shared_ptr<SymbolTable> parent;            // Pointer to the parent symbol table (for nested scopes)

public:
    // Constructor to create a new symbol table, optionally with a parent
    SymbolTable(std::shared_ptr<SymbolTable> parent = nullptr);

    // Retrieves a variable's value from the symbol table
    Value get(const std::string& name);

    // Sets a variable in the symbol table
    void set(const std::string& name, const Value& value);

    // Removes a variable from the symbol table
    void remove(const std::string& name);
};
