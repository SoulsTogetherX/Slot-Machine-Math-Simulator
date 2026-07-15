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
std::unordered_map<std::string, uint> StatsHandler::getSymbolCounts() const {
    return symbol_count;
}

std::string StatsHandler::serializeSymbols() const {
    std::string ret = "";
    uint total = 0;

    for (const auto& [key, value] : symbol_count) {
        ret += key + ": " + std::to_string(value) + "\n";
        total += value;
    }
    ret += "Total: " + std::to_string(total);
    
    return ret;
}
#pragma endregion

#pragma region Helper
void StatsHandler::merge(const StatsHandler &stats_handler) {
    count += stats_handler.count;

    for (const auto& [key, value] : stats_handler.symbol_count) {
        symbol_count[key] += value;
    }
}

void StatsHandler::clear() {
    count = 0;
    symbol_count.clear();
}
#pragma endregion