#include <iostream>
#include <algorithm>
#include <utility>

#include "main/slot_machine_tester.hpp"
#include "utils/extracts.hpp"



#pragma region Constructors
SlotMachineTester::SlotMachineTester() {
    bet_wager = 1;
    slot_machine = SlotMachine();
}
SlotMachineTester::SlotMachineTester(SlotMachine slots, uint bet_wager) {
    slot_machine = std::move(slots);
    this->bet_wager = bet_wager;
}
SlotMachineTester::SlotMachineTester(const nlohmann::json& info) {
    slot_machine = SlotMachine();
    loadJson(info);
}
#pragma endregion

#pragma region Load Info
void SlotMachineTester::loadJson(const nlohmann::json& info) {
    slot_machine.loadJson(info.at("slots_info"));

    printConfig();

    if (hasKey("test_info", info)) {
        const auto test_info = info.at("test_info");
        // Get bet amount
        bet_wager = extractInt("bet", test_info, 1);

        // Returns if no test spins requested
        if (!hasKey("test_spins", test_info)) {
            return;
        }

        // Spins requested test spins
        const auto test_spins = test_info.at("test_spins");
        if (test_spins.is_number_integer()) {
            runTestRun(test_spins.get<uint>());
        } else if (test_spins.is_array()) {
            auto rounds = std::vector<uint>();
            for(const auto& it : test_spins) {
                rounds.push_back(it);
            }
            runTestRun(rounds);
        } else {
            throw std::invalid_argument("Expected to find number or array, but found: " + to_string(info));
        }
    } else {
        // Default bet amount
        bet_wager = 1;
    }

}
#pragma endregion

#pragma region Test Runs
// Runs 'rounds' spins at the fixed bet_wager, folding each spin's payout into
// session, then prints the resulting report.
void SlotMachineTester::runTestRun(uint rounds) {
    for(uint i = 0; i < rounds; i++) {
        const auto betResult = slot_machine.spinBet();
        session.record(bet_wager, betResult.winPayout, betResult.losePayout);
    }

    std::cout << "  ===  " << "Rounds: " << rounds << "  ===  \n";
    printResults();
    std::cout << "\n\n\n";
}
// Convenience overload for running several batches back-to-back (e.g. staged testing).
void SlotMachineTester::runTestRun(const std::vector<uint>& roundsMass) {
    uint i = 0;
    for(uint rounds : roundsMass) {
        for(; i < rounds; i++) {
            const auto betResult = slot_machine.spinBet();
            session.record(bet_wager, betResult.winPayout, betResult.losePayout);
        }

        std::cout << "  ===  " << "Rounds: " << rounds << "  ===  \n";
        printResults();
        std::cout << "\n\n\n";
    }
}
#pragma endregion

#pragma region Config
// Prints a one-time snapshot of the loaded machine.
void SlotMachineTester::printConfig() const {
    const auto& symbol_handler = slot_machine.getSymbolHandler();
    const auto& reel_handler = slot_machine.getReelHandler();
    const auto& pattern_handler = slot_machine.getPatternHandler();
    const auto& paytable_handler = slot_machine.getPayTableHandler();

    std::cout << "===================  Machine Configuration  ===================\n";

    // Symbols
    auto symbol_ids = symbol_handler.getSymbolIds();
    std::sort(symbol_ids.begin(), symbol_ids.end());
    std::cout << "Symbols (" << symbol_ids.size() << "):\n";
    for (const auto& id : symbol_ids) {
        const Symbol& sym = symbol_handler.getSymbol(id);

        std::cout << "  " << id << "  [";
        switch (sym.getType()) {
            case NORMAL:  std::cout << "normal"; break;
            case WILD:    std::cout << "wild"; break;
            case SCATTER: std::cout << "scatter"; break;
            case BONUS:   std::cout << "bonus"; break;
            default:      std::cout << "unknown"; break;
        }
        std::cout << "]";

        if (sym.getMultiplier() != 1) {
            std::cout << "  x" << sym.getMultiplier() << " multiplier";
        }
        std::cout << "\n";
    }

    // Reels
    std::cout << "\nReels: " << reel_handler.getReelCount()
              << "  |  Payout rows (visible window): " << reel_handler.getPayoutRows() << "\n";
    for (uint i = 0; i < reel_handler.getReelCount(); i++) {
        std::cout << "  Reel " << (i + 1) << ": " << reel_handler.getReelLength(i) << " physical stop(s)\n";
    }

    // Patterns
    auto patterns = pattern_handler.getAllPatterns();
    std::sort(patterns.begin(), patterns.end(), [](const Pattern* a, const Pattern* b) {
        return a->getId() < b->getId();
    });
    std::cout << "\nPatterns (" << patterns.size() << "):\n";
    for (const Pattern* pattern : patterns) {
        std::cout << "  " << pattern->getId() << ": " << pattern->describe() << "\n";
    }

    // Paytables
    auto paytables = paytable_handler.getAllPayTables();
    std::sort(paytables.begin(), paytables.end(), [](const PayTable* a, const PayTable* b) {
        return a->getId() < b->getId();
    });
    std::cout << "\nPaytables (" << paytables.size() << "):\n";
    for (const PayTable* paytable : paytables) {
        std::cout << "  " << paytable->getId() << "  ->  pattern '" << paytable->getPatternId() << "'\n";
    }

    std::cout << "=================================================================\n\n";
}
#pragma endregion

#pragma region Results
// Prints the session's financial summary (RTP, volatility, hit frequency, etc.),
// the overall screen symbol distribution, and a per-paytable hit/consolation
// breakdown including each paytable's own contribution to the machine's RTP.
void SlotMachineTester::printResults() const {
    const auto& reel_handler = slot_machine.getReelHandler();
    const auto& paytable_handler = slot_machine.getPayTableHandler();
    
    const StatsHandler& reel_stats = reel_handler.getStats();
    const uint spins = reel_stats.getCount();

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "Spins:      " << spins << "\n";
    std::cout << "Wins:       " << session.getWinSpins() << "\n";
    std::cout << "Losses:     " << spins - session.getWinSpins() << "\n";
    std::cout << "Wager/spin: " << bet_wager << "\n\n";

    const double net_result = session.getTotalReturn() - session.getTotalWager();

    std::cout << "-- Financials --\n";
    std::cout << "Total wagered:     " << session.getTotalWager() << "\n";
    std::cout << "Total won:         " << session.getTotalWon() << "\n";
    std::cout << "Total consolation: " << session.getTotalConsolation() << "\n";
    std::cout << "Total returned:    " << session.getTotalReturn() << "\n";
    std::cout << "Net result:        " << (net_result >= 0 ? "+" : "") << net_result
              << "  (" << (net_result >= 0 ? "player up" : "house up") << ")\n";
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
    std::cout << reel_stats.serializeSymbols("  ") << "\n\n";

    auto paytables = paytable_handler.getAllPayTables();
    std::sort(paytables.begin(), paytables.end(), [](const PayTable* a, const PayTable* b) {
        return a->getId() < b->getId();
    });

    std::cout << "-- Paytable breakdown --\n";
    for (const PayTable* paytable : paytables) {
        const StatsHandler& pt_stats = paytable->getStats();
        const uint hits = pt_stats.getCount();
        const uint consolations = pt_stats.getConsolationCount();
        const double total_payout = pt_stats.getTotalPayout();
        const double total_consolation_payout = pt_stats.getTotalConsolationPayout();
        const double total_wagered = session.getTotalWager();

        std::cout << "  ==  " << paytable->getId() << "  (pattern '" << paytable->getPatternId() << "')  ==\n";

        std::cout << "  Wins:        " << hits;
        if (spins > 0) {
            std::cout << "  (" << (100.0 * hits / spins) << "% of spins)";
        }
        std::cout << "\n";

        std::cout << "  Consolation: " << consolations;
        if (spins > 0) {
            std::cout << "  (" << (100.0 * consolations / spins) << "% of spins)";
        }
        std::cout << "\n";

        std::cout << "  Payout awarded (wins):        " << total_payout << "\n";
        std::cout << "  Payout awarded (consolation): " << total_consolation_payout << "\n";
        if (total_wagered > 0) {
            std::cout << "  RTP contribution:             "
                      << (100.0 * total_payout / total_wagered) << "%"
                      << "  (+" << (100.0 * total_consolation_payout / total_wagered)
                      << "% consolation)\n";
        }
        if (hits > 0) {
            std::cout << "  Average payout per win:       " << (total_payout / hits) << "\n";
        }

        std::cout << "  Winning symbol distribution:\n" << pt_stats.serializeSymbols("    ") << "\n\n";
    }
}
#pragma endregion

#pragma region Helper
void SlotMachineTester::clear() {
    bet_wager = 1;
    slot_machine.clear();
    session.clear();
}
#pragma endregion
