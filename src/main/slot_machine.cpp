#include <nlohmann/json.hpp>

#include "utils/types.hpp"
#include "main/slot_machine.hpp"
#include "utils/extracts.hpp"
#include "virtual_reels/reel.hpp"
#include "pattern/pattern.hpp"



#pragma region Constructors
SlotMachine::SlotMachine() {
    clear();
}
SlotMachine::SlotMachine(const nlohmann::json& data) {
    loadJson(data);
}
#pragma endregion endregion

#pragma region Load Info
void SlotMachine::loadJson(const nlohmann::json& info) {
    clear();

    // Symbols
    symbol_handler.loadJson(info);

    // Virtual Reels
    reel_handler.loadJson(info, symbol_handler);

    // Patterns
    pattern_handler.loadJson(info, reel_handler);

    // Paytable
    paytable_handler.loadJson(info, pattern_handler);

    // Cache which patterns are actually scored, so spins convolve only those.
    referenced_patterns = paytable_handler.getReferencedPatternIds();
}
#pragma endregion

#pragma region Accessor Methods
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
SpinResult SlotMachine::spinBet() {
    auto screen = reel_handler.runSpin();
    auto cache = pattern_handler.convolveAll(screen, referenced_patterns);
    auto payouts = paytable_handler.evaluateAll(cache);

    double won = 0;
    double consolation = 0;
    for (const PayoutResult& result : payouts) {
        if (result.won) {
            won += result.payout;
        } else {
            consolation += result.payout;
        }
    }

    SpinResult ret;
    ret.winPayout = won;
    ret.losePayout = consolation;
    return ret;
}
#pragma endregion

#pragma region Helpers
// Clears all data
void SlotMachine::clear() {
    symbol_handler.clear();
    reel_handler.clear();
    pattern_handler.clear();
    paytable_handler.clear();

    referenced_patterns.clear();
}
#pragma endregion