#ifndef SYMBOL_HANDLER_HPP
#define SYMBOL_HANDLER_HPP

#include <vector>
#include <unordered_map>

#include "utilts/defs.hpp"
#include "symbol/symbol.hpp"



// The Class that registers and holds all used symbols for the slotmachine.
class SymbolHandler {
private:
    // A map of all registered Symbols (id, Symbol)
    std::unordered_map<string, Symbol> symbols;

    // Extracts Symbols from json.
    void extractSymbols(const json& info);
public:
    // Extracts Symbols from json.
    void loadJson(const json& info);

    // Accessor Methods
    bool hasSymbol(const string& id) const;
    const Symbol& getSymbol(const string& id) const;
    std::vector<string> getSymbolIds() const;

    // Clears Symbols
    void clear();
};

#endif  // SYMBOL_HANDLER_HPP
