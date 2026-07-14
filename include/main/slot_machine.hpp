#ifndef SLOT_MACHINE_HPP
#define SLOT_MACHINE_HPP

#include <vector>

#include "utilts/defs.hpp"
#include "symbol/symbol_handler.hpp"
#include "virtual_reels/reel_handler.hpp"
#include "pattern/pattern_handler.hpp"
#include "paytable/paytable_handler.hpp"
#include "stats/session_stats.hpp"



// The Main Class holding all information of a SlotMachine, and it's components.
class SlotMachine {
private:
    // Identifier
    string name;

    // Compoenent Classes
    SymbolHandler symbol_handler;
    ReelHandler reel_handler;
    PatternHandler pattern_handler;
    PayTableHandler paytable_handler;

    // The unique pattern ids referenced by at least one paytable, cached at load
    // so each spin convolves only the patterns that are actually scored.
    std::vector<string> referenced_patterns;

    // The total amount wagered on each spin (config "bet", default 1).
    double wager = 1;

    // Running bet statistics for the current session of spins.
    SessionStats session;
public:
    // Constructors
    SlotMachine();
    SlotMachine(string pathname);
    SlotMachine(const json& info);

    // Loads a SlotMachine's data from a json file.
    void loadPath(string pathname);
    void loadJson(const json& info);

    // Accessor Methods
    string getName() const;
    
    const SymbolHandler& getSymbolHandler() const;
    const ReelHandler& getReelHandler() const;
    const PatternHandler& getPatternHandler() const;
    const PayTableHandler& getPayTableHandler() const;

    const SessionStats& getSessionStats() const;

    // Spins the slotmachine once, tracking all stats along the way.
    void spinBet();

    // Prints the raw statistics
    void printRaw() const;

    // Clears all Compoenent Classes
    void clear();
};

#endif  // SLOT_MACHINE_HPP
