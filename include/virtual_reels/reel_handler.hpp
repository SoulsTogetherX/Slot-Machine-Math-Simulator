#ifndef VIRTUAL_REEL_HANDLER_HPP
#define VIRTUAL_REEL_HANDLER_HPP

#include <vector>
#include <random>
#include <nlohmann/json.hpp>

#include "utils/types.hpp"
#include "utils/symbol_types.hpp"
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

    // Extracts Symbols from nlohmann::json.
    void extractPayoutRows(const nlohmann::json& info);
    void extractSeed(const nlohmann::json& info);
    void extractReels(const nlohmann::json& info, const SymbolHandler& symbol_handler);
public:
    // Extracts Symbols from nlohmann::json.
    void loadJson(const nlohmann::json& info, const SymbolHandler& symbol_handler);

    // Returns a slotmachine spin and returns the screen, via an array of symbols
    SymbolGrid runSpin();

    uint getReelCount() const;
    uint getReelLength(uint reelNum) const;
    uint getMaxReelLength() const;
    uint getPayoutRows() const;
    SymbolGrid getSpinResult();

    const StatsHandler& getStats() const;

    // Clears Reels and stats
    void clear();
};

#endif  // VIRTUAL_REEL_HANDLER_HPP
