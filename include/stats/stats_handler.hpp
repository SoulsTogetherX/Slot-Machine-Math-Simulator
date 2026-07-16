#ifndef STATS_HANDLER_HPP
#define STATS_HANDLER_HPP

#include <unordered_map>
#include <vector>

#include "utils/types.hpp"
#include "utils/symbol_types.hpp"
#include "symbol/symbol_handler.hpp"



// The Class responsible for storing the stat data of other classes
class StatsHandler {
private:
    // The amount of times the attached object has activated successfully
    uint count = 0;

    // The count each symbol has been successfully referenced in this object
    std::unordered_map<std::string, uint> symbol_count;

    // The amount of times the attached object activated as a non-winning
    // (consolation) result.
    uint consolation_count = 0;

    // The cumulative credits paid out by the attached object, split the same
    // way as above: 'total_payout' for wins, 'total_consolation_payout' for
    // non-winning consolation payouts.
    double total_payout = 0;
    double total_consolation_payout = 0;
public:
    // Merges another StatsHandler object's data to this one.
    // Does not affect the given object
    void merge(const StatsHandler &stats_handler);

    // Registers all registered symbols to '0' count
    void registerSymbols(const SymbolHandler &symbol_handler);

    // Increases the success count
    void increaseCount(uint inc = 0);
    // Increases the consolation (non-winning) activation count
    void increaseConsolationCount(uint inc = 0);

    // Adds to the cumulative payout totals
    void addPayout(double amount);
    void addConsolationPayout(double amount);

    // Add to the count of recorded symbols
    void addSymbol(const Symbol* sym);
    void addSymbols(const SymbolLine& syms);
    void addSymbolMass(const SymbolGrid& syms);

    uint getCount() const;
    uint getConsolationCount() const;
    double getTotalPayout() const;
    double getTotalConsolationPayout() const;
    std::unordered_map<std::string, uint> getSymbolCounts() const;

    // Returns a human-readable, newline-separated listing of every symbol count,
    // along with each symbol's share of the total as a percentage.
    std::string serializeSymbols(const std::string &indent = "") const;

    // Clears all stat information
    void clear();
};

#endif  // STATS_HANDLER_HPP