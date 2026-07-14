#include <fstream>
#include <iostream>
#include <iomanip>

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

    // Wager placed per spin
    wager = extractInt("bet", data, 1);

    // Symbols
    symbol_handler.loadJson(data);

    // Virtual Reels
    reel_handler.loadJson(data, symbol_handler);

    // Patterns
    pattern_handler.loadJson(data, reel_handler);

    // Paytable
    paytable_handler.loadJson(data, pattern_handler);

    // Cache which patterns are actually scored, so spins convolve only those.
    referenced_patterns = paytable_handler.getReferencedPatternIds();
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

const SessionStats& SlotMachine::getSessionStats() const {
    return session;
}
#pragma endregion

#pragma region Runner
// Spins the slot machine once
void SlotMachine::spinBet() {
    auto screen = reel_handler.runSpin();
    auto cache = pattern_handler.convolveAll(screen, referenced_patterns);
    auto payouts = paytable_handler.evaluateAll(cache);

    // Split this spin's return into won money and consolation money.
    double won = 0;
    double consolation = 0;
    for (const PayoutResult& result : payouts) {
        if (result.won) {
            won += result.payout;
        } else {
            consolation += result.payout;
        }
    }
    session.record(wager, won, consolation);
}
#pragma endregion

#pragma region Results
void SlotMachine::printRaw() const {
    const StatsHandler& reel_stats = reel_handler.getStats();
    const uint spins = reel_stats.getCount();

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "===== " << name << " =====\n";
    std::cout << "Spins:      " << spins << "\n";
    std::cout << "Wager/spin: " << wager << "\n\n";

    std::cout << "-- Financials --\n";
    std::cout << "Total wagered:     " << session.getTotalWager() << "\n";
    std::cout << "Total won:         " << session.getTotalWon() << "\n";
    std::cout << "Total consolation: " << session.getTotalConsolation() << "\n";
    std::cout << "Total returned:    " << session.getTotalReturn() << "\n";
    std::cout << "Observed RTP:      " << (100.0 * session.rtp()) << "%"
              << "  (won " << (100.0 * session.rtpWon()) << "%"
              << ", consolation " << (100.0 * session.rtpConsolation()) << "%)\n";
    std::cout << "Hit frequency:     " << (100.0 * session.hitFrequency()) << "%\n";
    std::cout << "Average win:       " << session.avgWin() << "\n";
    std::cout << "Max win:           " << session.getMaxWin() << "\n";
    std::cout << "Longest loss run:  " << session.getMaxLossStreak() << "\n";
    std::cout << "Std deviation:     " << session.stddev() << "\n";
    std::cout << "Volatility index:  " << session.volatilityIndex() << "  (95%)\n";
    std::cout << "RTP 95% CI:        " << (100.0 * session.rtp()) << "% +/- "
              << (100.0 * session.rtpMargin()) << "%\n\n";

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

    referenced_patterns.clear();
    wager = 1;
    session.clear();
}
#pragma endregion