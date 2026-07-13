#ifndef VIRTUAL_REEL_HPP
#define VIRTUAL_REEL_HPP

#include <vector>
#include <random>

#include "utilts/defs.hpp"
#include "symbol/symbol.hpp"



class VirtualReel {
private:
    std::vector<const Symbol*> reel;
    std::uniform_int_distribution<int> dist;

    uint currentPos = 0;
public:
    void spin(std::mt19937 &rng);
    Symbol getSymbolAt(uint idx) const;

    void addSymbol(const Symbol &s, uint repeat);

    uint getReelLength() const;
};

#endif  // VIRTUAL_REEL_HPP
