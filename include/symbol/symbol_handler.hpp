#ifndef SYMBOL_HANDLER_HPP
#define SYMBOL_HANDLER_HPP

#include <vector>
#include <unordered_map>

#include <nlohmann/json.hpp>
#include "utils/types.hpp"
#include "symbol/symbol.hpp"



// The Class that registers and holds all used symbols for the slotmachine.
class SymbolHandler {
private:
    // A map of all registered Symbols (id, Symbol)
    std::unordered_map<std::string, Symbol> symbols;

    // Extracts Symbols from nlohmann::json.
    void extractSymbols(const nlohmann::json& info);
public:
    // Extracts Symbols from nlohmann::json.
    void loadJson(const nlohmann::json& info);

    // Accessor Methods
    bool hasSymbol(const std::string& id) const;
    const Symbol& getSymbol(const std::string& id) const;
    std::vector<std::string> getSymbolIds() const;

    // Clears Symbols
    void clear();
};

#endif  // SYMBOL_HANDLER_HPP
