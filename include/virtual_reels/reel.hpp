#ifndef VIRTUAL_REEL_HPP
#define VIRTUAL_REEL_HPP

#include <vector>
#include <random>

#include "utilts/defs.hpp"
#include "symbol/symbol.hpp"



// A class representing the Virtual Reel of a slotmachine.
class VirtualReel {
private:
    std::vector<const Symbol*> reel; // Symbols on the Reel, in order.
    std::uniform_int_distribution<int> dist;

    uint currentPos = 0; // Current Position on the Reel
public:
    // Spins the Reel once
    void spin(std::mt19937 &rng);

    // Adds a symbol to the Reel
    void addSymbol(const Symbol &s, uint repeat);

    // Accessor Methods
    uint getReelLength() const;
    // Returns a shared, non-owning pointer to the Symbol at the given offset.
    const Symbol* getSymbolAt(uint idx) const;
};

#endif  // VIRTUAL_REEL_HPP
