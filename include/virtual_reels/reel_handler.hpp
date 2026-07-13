#ifndef VIRTUAL_REEL_HANDLER_HPP
#define VIRTUAL_REEL_HANDLER_HPP

#include <vector>
#include <random>

#include "utilts/defs.hpp"
#include "symbol/symbol_handler.hpp"
#include "virtual_reels/reel.hpp"



class ReelHandler {
private:
    uint payoutRows;
    int seed;

    std::mt19937 rng;
    std::vector<VirtualReel> reels;

    void extractPayoutRows(const json& info);
    void extractSeed(const json& info);
    void extractReels(const json& info, const SymbolHandler& symbol_handler);

    void seedFromHardware();
public:
    void loadJson(const json& info, const SymbolHandler& symbol_handler);

    std::vector<std::vector<Symbol>> runSpin();
    std::vector<std::vector<Symbol>> getPrevResult();

    uint getReelCount() const;
    uint getReelLength(uint reelNum) const;
    uint getMaxReelLength() const;
    uint getPayoutRows() const;

    void clear();
};


#endif  // VIRTUAL_REEL_HANDLER_HPP
