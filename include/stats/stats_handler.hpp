#ifndef STATS_HANDLER_HPP
#define STATS_HANDLER_HPP

#include <unordered_map>
#include <vector>

#include "utilts/defs.hpp"
#include "symbol/symbol_handler.hpp"



// The Class responsible for storing the stat data of other classes
class StatsHandler {
private:
    // The amount of times the attached object has activated successfully
    uint count = 0;

    // The count each symbol has been successfully referenced in this object
    std::unordered_map<string, uint> symbolCount;
public:
    // Merges another StatsHandler object's data to this one.
    // Does not affect the given object
    void merge(const StatsHandler &stats_handler);

    // Registers all registered symbols to '0' count
    void registerSymbols(const SymbolHandler &symbol_handler);

    // Increases the count
    void increaseCount(uint inc = 0);

    // Add to the count of recorded symbols
    void addSymbol(const Symbol& sym);
    void addSymbols(const std::vector<Symbol>& syms);
    void addSymbolMass(const std::vector<std::vector<Symbol>>& syms);

    // Accessor Methods
    uint getCount() const;
    std::unordered_map<string, uint> getSymbolCounts() const;

    // Clears all stat information
    void clear();
};

#endif  // STATS_HANDLER_HPP