#ifndef VIRTUAL_REEL_HANDLER_HPP
#define VIRTUAL_REEL_HANDLER_HPP

#include <vector>
#include <random>

#include "utilts/defs.hpp"
#include "stats/stats_handler.hpp"
#include "symbol/symbol_handler.hpp"
#include "virtual_reels/reel.hpp"



// A Class used to store reels and return a whole slotmachine screen, after spins.
class ReelHandler {
private:
    StatsHandler stats;

    // Screen Info
    uint payoutRows;
    std::vector<VirtualReel> reels;

    // RNG
    std::mt19937 rng;
    void seedFromHardware();

    // Extracts Symbols from json.
    void extractPayoutRows(const json& info);
    void extractSeed(const json& info);
    void extractReels(const json& info, const SymbolHandler& symbol_handler);
public:
    // Extracts Symbols from json.
    void loadJson(const json& info, const SymbolHandler& symbol_handler);

    // Returns a slotmachine spin and returns the screen, via an array of symbols
    std::vector<std::vector<Symbol>> runSpin();

    // Accesor Methods
    uint getReelCount() const;
    uint getReelLength(uint reelNum) const;
    uint getMaxReelLength() const;
    uint getPayoutRows() const;
    std::vector<std::vector<Symbol>> getSpinResult();

    StatsHandler getStats() const;

    // Clears Reels and stats
    void clear();
};

#endif  // VIRTUAL_REEL_HANDLER_HPP
