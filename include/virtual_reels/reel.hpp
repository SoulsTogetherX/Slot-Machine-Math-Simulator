#ifndef VIRTUAL_REEL_HPP
#define VIRTUAL_REEL_HPP

#include <vector>
#include <random>

#include "utilts/defs.hpp"
#include "symbol/symbol.hpp"



// A class representing a single Virtual Reel of a slotmachine.
class VirtualReel {
private:
    std::vector<const Symbol*> strip; // Physical band, in order (defines adjacency).
    std::vector<uint> cum_weights;    // Running total of virtual stops, one per stop.
    uint total_weight = 0;            // Total number of virtual stops on the reel.

    std::uniform_int_distribution<uint> dist;
    uint current_pos = 0;             // Current physical stop index.
public:
    // Spins the Reel once, landing on a physical stop chosen by virtual-stop weight.
    void spin(std::mt19937 &rng);

    // Appends one physical stop of the symbol carrying 'weight' virtual stops.
    void addStop(const Symbol &s, uint weight);

    uint getStripLength() const;   // Number of physical stops (the reel's length).
    uint getTotalWeight() const;   // Number of virtual stops.
    
    // Returns a shared, non-owning pointer to the Symbol 'idx' stops below the landed stop.
    const Symbol* getSymbolAt(uint idx) const;
};

#endif  // VIRTUAL_REEL_HPP
