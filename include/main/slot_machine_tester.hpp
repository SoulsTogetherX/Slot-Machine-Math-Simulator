#ifndef SLOT_MACHINE_TESTER_HPP
#define SLOT_MACHINE_TESTER_HPP

#include <vector>

#include <nlohmann/json.hpp>
#include "utils/types.hpp"
#include "main/slot_machine.hpp"



// Drives a SlotMachine through batches of spins at a fixed wager and reports the
// resulting RTP/volatility/stat summary; the "test harness" layer above SlotMachine.
class SlotMachineTester {
private:
    // Identifier
    SlotMachine slot_machine;

    // The base wager: how much is staked on each spin.
    uint bet_wager;

    // Running bet statistics for the current session of spins.
    SessionStats session;
public:
    SlotMachineTester();
    SlotMachineTester(SlotMachine slots, uint bet_wager);
    SlotMachineTester(const nlohmann::json& info);

    // Loads a SlotMachine's data from a nlohmann::json object.
    void loadJson(const nlohmann::json& info);

    // Runs 'rounds' spins, or several batches in sequence, accumulating into session.
    void runTestRun(uint rounds);
    void runTestRun(std::vector<uint> rounds);

    // Prints the accumulated financial and symbol/paytable statistics to stdout.
    void printResults() const;

    // Clears all Compoenent Classes
    void clear();
};

#endif  // SLOT_MACHINE_TESTER_HPP
