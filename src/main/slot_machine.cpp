#include <fstream>

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
#pragma endregion

#pragma region Runner
// Spins the slot machine once
void SlotMachine::spinBet() {
    auto results = reel_handler.runSpin();
    paytable_handler.evaluateAll(results, pattern_handler);
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