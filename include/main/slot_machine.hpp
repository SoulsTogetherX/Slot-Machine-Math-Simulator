#ifndef SLOT_MACHINE_HPP
#define SLOT_MACHINE_HPP

#include "utilts/defs.hpp"
#include "symbol/symbol_handler.hpp"
#include "virtual_reels/reel_handler.hpp"
#include "pattern/pattern_handler.hpp"
#include "paytable/paytable_handler.hpp"



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

    // Spins the slotmachine once, tracking all stats along the way.
    void spinBet();

    // Clears all Compoenent Classes
    void clear();
};

#endif  // SLOT_MACHINE_HPP
