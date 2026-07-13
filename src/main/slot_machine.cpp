#include <fstream>
#include <iostream>

#include "utilts/defs.hpp"
#include "main/slot_machine.hpp"
#include "utilts/extracts.hpp"
#include "virtual_reels/reel.hpp"
#include "pattern/pattern.hpp"



#pragma region Constructors
SlotMachine::SlotMachine() {
    clear();
}
SlotMachine::SlotMachine(string pathname) {
    loadPath(pathname);
}
SlotMachine::SlotMachine(const json& data) {
    loadJson(data);
}
#pragma endregion endregion

#pragma region Load Info
void SlotMachine::loadPath(string pathname) {
    clear();

    std::ifstream file(pathname);
    if (!file) {
        throw std::runtime_error("Could not open file: " + pathname);
    }

    json data;
    file >> data;
    loadJson(data);
}
void SlotMachine::loadJson(const json& data) {
    clear();
    
    // Id
    name = extractStr("name", data, "default-slotmachine-name");

    // Symbols
    symbol_handler.loadJson(data);

    // Virtual Reels
    reel_handler.loadJson(data, symbol_handler);

    // Patterns
    pattern_handler.loadJson(data, reel_handler);

    // Paytable
    paytable_handler.loadJson(data, pattern_handler);
}
#pragma endregion

#pragma region Accessor Methods
string SlotMachine::getName() const {
    return name;
}

const SymbolHandler& SlotMachine::getSymbolHandler() const {
    return symbol_handler;
}
const ReelHandler& SlotMachine::getReelHandler() const {
    return reel_handler;
}
const PatternHandler& SlotMachine::getPatternHandler() const {
    return pattern_handler;
}
const PayTableHandler& SlotMachine::getPayTableHandler() const {
    return paytable_handler;
}
#pragma endregion

#pragma region Runner
// Spins the slot machine once
void SlotMachine::spinBet() {
    auto results = reel_handler.runSpin();
    paytable_handler.evaluateAll(results, pattern_handler);
}
#pragma endregion

#pragma region Results
void SlotMachine::printRaw() const {
    const StatsHandler& reel_stats = reel_handler.getStats();
    const uint spins = reel_stats.getCount();

    std::cout << "===== " << name << " =====\n";
    std::cout << "Spins: " << spins << "\n\n";

    std::cout << "Screen symbol distribution:\n";
    std::cout << reel_stats.serializeSymbols() << "\n";

    std::cout << "Paytable hits:\n";
    for (const PayTable* paytable : paytable_handler.getAllPayTables()) {
        const uint hits = paytable->getStats().getCount();
        std::cout << "  " << paytable->getId() << ": " << hits << " hit(s)";
        if (spins > 0) {
            std::cout << " (" << (100.0 * hits / spins) << "% of spins)";
        }
        std::cout << "\n";
    }
}
#pragma endregion

#pragma region Helpers
// Clears all data
void SlotMachine::clear() {
    name = "";

    symbol_handler.clear();
    reel_handler.clear();
    pattern_handler.clear();
    paytable_handler.clear();
}
#pragma endregion