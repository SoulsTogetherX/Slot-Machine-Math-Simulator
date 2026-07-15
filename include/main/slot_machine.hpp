#ifndef SLOT_MACHINE_HPP
#define SLOT_MACHINE_HPP

#include <vector>

#include <nlohmann/json.hpp>
#include "utils/types.hpp"
#include "symbol/symbol_handler.hpp"
#include "virtual_reels/reel_handler.hpp"
#include "pattern/pattern_handler.hpp"
#include "paytable/paytable_handler.hpp"
#include "stats/session_stats.hpp"



// The outcome of one spin: total credits from winning paytable hits (winPayout)
// and total credits from non-winning/consolation hits (losePayout).
struct SpinResult {
    double winPayout;
    double losePayout;
};


// The Main Class holding all information of a SlotMachine, and it's components.
class SlotMachine {
private:
    SymbolHandler symbol_handler;
    ReelHandler reel_handler;
    PatternHandler pattern_handler;
    PayTableHandler paytable_handler;

    // The unique pattern ids referenced by at least one paytable, cached at load
    // so each spin convolves only the patterns that are actually scored.
    std::vector<std::string> referenced_patterns;
public:
    SlotMachine();
    SlotMachine(const nlohmann::json& info);

    // Loads a SlotMachine's data from a nlohmann::json object.
    void loadJson(const nlohmann::json& info);
    
    const SymbolHandler& getSymbolHandler() const;
    const ReelHandler& getReelHandler() const;
    const PatternHandler& getPatternHandler() const;
    const PayTableHandler& getPayTableHandler() const;

    // Spins the slotmachine once, tracking all stats along the way.
    SpinResult spinBet();

    // Clears all Compoenent Classes
    void clear();
};

#endif  // SLOT_MACHINE_HPP
