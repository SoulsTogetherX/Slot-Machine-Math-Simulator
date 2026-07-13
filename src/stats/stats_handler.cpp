#include "stats/stats_handler.hpp"


void StatsHandler::merge(const StatsHandler &stats_handler) {
    auto other_symbol_count = stats_handler.getSymbolCounts();
    count = stats_handler.count;
    
    for (auto pairs : symbolCount) {
        pairs.second += other_symbol_count[pairs.first];
    }
}
void StatsHandler::registerSymbols(const SymbolHandler &symbol_handler) {
    for (auto id : symbol_handler.getSymbolIds()) {
        symbolCount[id] = 0;
    }
}

void StatsHandler::increaseCount(uint inc) {
    count += inc;
}

void StatsHandler::addSymbol(const Symbol& sym) {
    symbolCount[sym.getId()] += 1;
}
void StatsHandler::addSymbols(const std::vector<Symbol>& syms) {
    for (auto sym : syms) {
        symbolCount[sym.getId()] += 1;
    }
}
void StatsHandler::addSymbolMass(const std::vector<std::vector<Symbol>>& syms) {
    for (auto row : syms) {
        for (auto sym : row) {
            symbolCount[sym.getId()] += 1;
        }
    }
}

uint StatsHandler::getCount() const {
    return count;
}
std::unordered_map<string, uint> StatsHandler::getSymbolCounts() const {
    return symbolCount;
}

void StatsHandler::clear() {
    count = 0;
    symbolCount.clear();
}