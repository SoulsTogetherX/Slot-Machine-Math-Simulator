#include <iostream>
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
#pragma endregion endregion

#pragma region Load Info
// Reads the flat "bet" field (defaults to 1) and delegates the rest of the config to SlotMachine.
void SlotMachineTester::loadJson(const nlohmann::json& info) {
    slot_machine.loadJson(info.at("slots_info"));
    
    if (hasKey("test_info", info)) {
        auto test_info = info.at("test_info");
        // Get bet amount
        bet_wager = extractInt("bet", test_info, 1);

        // Returns if no test spins requested
        if (!hasKey("test_spins", test_info)) {
            return;
        }

        // Spins requested test spins
        auto test_spins = test_info.at("test_spins");
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
#pragma endregion endregion

#pragma region Test Runs
// Runs 'rounds' spins at the fixed bet_wager, folding each spin's payout into session.
void SlotMachineTester::runTestRun(uint rounds) {
    for(uint i = 0; i < rounds; i++) {
        auto betResult = slot_machine.spinBet();
        session.record(bet_wager, betResult.winPayout, betResult.losePayout);
    }
}
// Convenience overload for running several batches back-to-back (e.g. staged testing).
void SlotMachineTester::runTestRun(const std::vector<uint>& roundsMass) {
    uint i = 0;
    for(uint rounds : roundsMass) {
        for(; i < rounds; i++) {
            auto betResult = slot_machine.spinBet();
            session.record(bet_wager, betResult.winPayout, betResult.losePayout);
        }

        std::cout << "  ===  " << "Rounds: " << rounds << "  ===  \n";
        printResults();
        std::cout << "\n\n\n";
    }
}
#pragma endregion endregion

#pragma region Results
// Prints the session's financial summary (RTP, volatility, hit frequency, etc.),
// the overall screen symbol distribution, and a per-paytable hit breakdown.
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
    std::cout << reel_stats.serializeSymbols() << "\n\n";

    std::cout << "\tPaytable hits:\n";
    for (const PayTable* paytable : paytable_handler.getAllPayTables()) {
        const uint hits = paytable->getStats().getCount();
        std::cout << "  ==  " << paytable->getId() << "  ==  \n";
        std::cout << hits << " hit(s)";
        if (spins > 0) {
            std::cout << " (" << (100.0 * hits / spins) << "% of spins)";
        }
        std::cout << "\n" << paytable->getStats().serializeSymbols() << "\n";
    }
}
#pragma endregion endregion

#pragma region Helper
void SlotMachineTester::clear() {
    bet_wager = 1;
    slot_machine.clear();
    session.clear();
}
#pragma endregion endregion