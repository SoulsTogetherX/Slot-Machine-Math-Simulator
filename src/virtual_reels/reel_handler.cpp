#include <nlohmann/json.hpp>

#include "virtual_reels/reel_handler.hpp"
#include "utils/types.hpp"
#include "utils/extracts.hpp"



#pragma region Extract Info Helpers
void ReelHandler::loadJson(
    const nlohmann::json& info, const SymbolHandler& symbol_handler
) {
    extractPayoutRows(info.at("payout_rows"));
    extractSeed(info);
    extractReels(info.at("reels"), symbol_handler);
    stats.registerSymbols(symbol_handler);
}

void ReelHandler::extractPayoutRows(const nlohmann::json& info) {
    if (!info.is_number_integer()) {
        throw std::invalid_argument("Reels is expected to be integer, but found: " + to_string(info));
    }

    payoutRows = info.get<int>();
}

void ReelHandler::extractSeed(const nlohmann::json& info) {
    if (!hasKey("seed", info)) {
        seedFromHardware();
        return;
    }

    const nlohmann::json& seed_info = info.at("seed");

    if (seed_info.is_number_integer()) {
        rng.seed(static_cast<std::mt19937::result_type>(seed_info.get<int>()));
        return;
    } else if (seed_info.is_string()) {
        const std::string str_check = seed_info.get<std::string>();

        if (str_check == "auto") {
            seedFromHardware();
            return;
        }
    }

    throw std::invalid_argument("Seed must be either an integer or 'auto', but found: " + to_string(seed_info));
}

void ReelHandler::extractReels(const nlohmann::json& info, const SymbolHandler& symbol_handler) {
    if (!info.is_array()) {
        throw std::invalid_argument("Reels is expected to be array, but found: " + to_string(info));
    }
    reels = std::vector<VirtualReel>(info.size(), VirtualReel());

    for(size_t i = 0; i < info.size(); i++) {
        auto& reel = reels[i];
        const auto& reel_data = info.at(i);

        if (!reel_data.is_array()) {
            throw std::invalid_argument("'reels' can only contain either a string or object, but found: " + to_string(reel_data));
        }
        
        for(const auto& symbol_data : reel_data) {
            if (symbol_data.is_string()) {
                std::string id = symbol_data.get<std::string>();
                if (!symbol_handler.hasSymbol(id)) {
                    throw std::invalid_argument("Symbol '" + id + "' not defined in symbol table.");
                }

                reel.addStop(symbol_handler.getSymbol(id), 1);
                continue;
            } else if (symbol_data.is_object()) {
                std::string id = extractStr("id", symbol_data);
                if (!symbol_handler.hasSymbol(id)) {
                    throw std::invalid_argument("Symbol '" + id + "' not defined in symbol table.");
                }

                // 'repeat' adds this many consecutive physical stops.
                // 'weight' is the virtual-stop count given to each of those physical stops.
                const int repeat = extractInt("repeat", symbol_data, 1);
                const int weight = extractInt("weight", symbol_data, 1);
                if (repeat < 0 || weight < 0) {
                    throw std::invalid_argument("'repeat' and 'weight' must be non-negative.");
                }

                const Symbol& sym = symbol_handler.getSymbol(id);
                for (int k = 0; k < repeat; k++) {
                    reel.addStop(sym, static_cast<uint>(weight));
                }
                continue;
            }

            throw std::invalid_argument("'reels' can only contain either a string or object, but found: " + to_string(symbol_data));
        }
    }
}
#pragma endregion

#pragma region Run Methods
// Spins every reel independently, then reads off the resulting on-screen grid and
// tallies it into this handler's stats.
SymbolGrid ReelHandler::runSpin() {
    for(uint i = 0; i < reels.size(); i++) {
        reels[i].spin(rng);
    }

    const auto results = getSpinResult();
    stats.addSymbolMass(results);
    stats.increaseCount(1);
    return results;
}
#pragma endregion

#pragma region Accessor Methods
// Reads the currently visible window (payoutRows) off each reel's landed stop.
SymbolGrid ReelHandler::getSpinResult() {
    auto ret = SymbolGrid(reels.size(), SymbolLine());

    for(uint i = 0; i < reels.size(); i++) {
        const auto& reel = reels[i];
        auto& row = ret[i];

        row.resize(payoutRows);
        for(uint r = 0; r < payoutRows; r++) {
            row[r] = reel.getSymbolAt(r);
        }
    }

    return ret;
}

uint ReelHandler::getReelCount() const {
    return reels.size();
}
uint ReelHandler::getPayoutRows() const {
    return payoutRows;
}
uint ReelHandler::getReelLength(uint reelNum) const {
    return reels[reelNum].getStripLength();
}
uint ReelHandler::getMaxReelLength() const {
    uint ret = 0;
    for(uint i = 0; i < getReelCount(); i++) {
        const uint temp = getReelLength(i);
        ret = ret > temp ? ret : temp;
    }
    return ret;
}

const StatsHandler& ReelHandler::getStats() const {
    return stats;
}
#pragma endregion

#pragma region Helper
void ReelHandler::seedFromHardware() {
    std::random_device rd;
    rng.seed(rd());
}
void ReelHandler::clear() {
    payoutRows = 0;
    stats.clear();
    reels.clear();
}
#pragma endregion