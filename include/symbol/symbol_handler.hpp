#ifndef SYMBOL_HANDLER_HPP
#define SYMBOL_HANDLER_HPP

#include <vector>
#include <unordered_map>

#include "utilts/defs.hpp"
#include "symbol/symbol.hpp"



class SymbolHandler {
private:
    std::unordered_map<string, Symbol> symbols;

    void extractSymbols(const json& info);
public:
    void loadJson(const json& info);

    bool hasSymbol(string id) const;
    const Symbol& getSymbol(string id) const;

    std::vector<string> getSymbolIds() const;

    void clear();
};

#endif  // SYMBOL_HANDLER_HPP
