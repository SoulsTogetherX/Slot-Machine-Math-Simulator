#ifndef STATS_HANDLER_HPP
#define STATS_HANDLER_HPP

#include <unordered_map>
#include <vector>

#include "utilts/defs.hpp"
#include "symbol/symbol_handler.hpp"



class StatsHandler {
private:
    uint count = 0;

    std::unordered_map<string, uint> symbolCount;
public:
    void merge(const StatsHandler &stats_handler);

    void registerSymbols(const SymbolHandler &symbol_handler);

    void increaseCount(uint inc = 0);

    void addSymbol(const Symbol& sym);
    void addSymbols(const std::vector<Symbol>& syms);
    void addSymbolMass(const std::vector<std::vector<Symbol>>& syms);

    uint getCount() const;
    std::unordered_map<string, uint> getSymbolCounts() const;

    void clear();
};

#endif  // STATS_HANDLER_HPP