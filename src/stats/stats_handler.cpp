#include <algorithm>
#include <sstream>
#include <iomanip>
#include <vector>

#include "stats/stats_handler.hpp"



#pragma region Init Methods
void StatsHandler::registerSymbols(const SymbolHandler &symbol_handler) {
    for (const auto& id : symbol_handler.getSymbolIds()) {
        symbol_count[id] = 0;
    }
}
#pragma endregion

#pragma region Stat Inc Methods
void StatsHandler::increaseCount(uint inc) {
    count += inc;
}
void StatsHandler::increaseConsolationCount(uint inc) {
    consolation_count += inc;
}

void StatsHandler::addPayout(double amount) {
    total_payout += amount;
}
void StatsHandler::addConsolationPayout(double amount) {
    total_consolation_payout += amount;
}

void StatsHandler::addSymbol(const Symbol* sym) {
    symbol_count[sym->getId()] += 1;
}
void StatsHandler::addSymbols(const SymbolLine& syms) {
    for (const Symbol* sym : syms) {
        symbol_count[sym->getId()] += 1;
    }
}
void StatsHandler::addSymbolMass(const SymbolGrid& syms) {
    for (const SymbolLine& row : syms) {
        for (const Symbol* sym : row) {
            symbol_count[sym->getId()] += 1;
        }
    }
}
#pragma endregion

#pragma region Accessor Methods
uint StatsHandler::getCount() const {
    return count;
}
uint StatsHandler::getConsolationCount() const {
    return consolation_count;
}
double StatsHandler::getTotalPayout() const {
    return total_payout;
}
double StatsHandler::getTotalConsolationPayout() const {
    return total_consolation_payout;
}
std::unordered_map<std::string, uint> StatsHandler::getSymbolCounts() const {
    return symbol_count;
}

std::string StatsHandler::serializeSymbols(const std::string &indent) const {
    uint total = 0;
    std::vector<std::string> keys;

    keys.reserve(symbol_count.size());
    for (const auto& [key, value] : symbol_count) {
        keys.push_back(key);
        total += value;
    }
    std::sort(keys.begin(), keys.end());

    std::ostringstream out;
    out << std::fixed << std::setprecision(2);
    for (const auto& key : keys) {
        const uint value = symbol_count.at(key);
        const double pct = total > 0 ? (100.0 * value / total) : 0.0;
        out << indent << key << ": " << value << "  (" << pct << "%)\n";
    }
    out << indent << "Total: " << total;

    return out.str();
}
#pragma endregion

#pragma region Helper
void StatsHandler::merge(const StatsHandler &stats_handler) {
    count += stats_handler.count;
    consolation_count += stats_handler.consolation_count;
    total_payout += stats_handler.total_payout;
    total_consolation_payout += stats_handler.total_consolation_payout;

    for (const auto& [key, value] : stats_handler.symbol_count) {
        symbol_count[key] += value;
    }
}

void StatsHandler::clear() {
    count = 0;
    consolation_count = 0;
    total_payout = 0;
    total_consolation_payout = 0;
    symbol_count.clear();
}
#pragma endregion