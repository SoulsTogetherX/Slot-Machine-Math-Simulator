#include "virtual_reels/reel_handler.hpp"
#include "utilts/defs.hpp"
#include "utilts/extracts.hpp"



#pragma region Extract Info Helpers
void ReelHandler::loadJson(
    const json& info, const SymbolHandler& symbol_handler
) {
    extractPayoutRows(info.at("payout_rows"));
    extractSeed(info);
    extractReels(info.at("reels"), symbol_handler);
}

void ReelHandler::extractPayoutRows(const json& info) {
    if (!info.is_number_integer()) {
        throw std::invalid_argument("Reels is expected to be integer, but found: " + to_string(info));
    }

    payoutRows = info.get<int>();
}

void ReelHandler::extractSeed(const json& info) {
    if (!info.contains("seed") || info.at("seed").is_null()) {
        seedFromHardware();
        return;
    }

    const json& seed_info = info.at("seed");

    if (seed_info.is_number_integer()) {
        rng.seed(static_cast<std::mt19937::result_type>(seed_info.get<int>()));
        return;
    } else if (seed_info.is_string()) {
        string str_check = seed_info.get<string>();

        if (str_check == "auto") {
            seedFromHardware();
            return;
        }
    }

    throw std::invalid_argument("Seed must be either an integer or 'auto', but found: " + to_string(seed_info));
}

void ReelHandler::extractReels(const json& info, const SymbolHandler& symbol_handler) {
    if (!info.is_array()) {
        throw std::invalid_argument("Reels is expected to be array, but found: " + to_string(info));
    }
    reels = std::vector<VirtualReel>(info.size(), VirtualReel());

    for(size_t i = 0; i < info.size(); i++) {
        auto& reel = reels[i];
        auto reel_data = info.at(i);

        if (!reel_data.is_array()) {
            throw std::invalid_argument("'reels' can only contain either a string or object, but found: " + to_string(reel_data));
        }
        
        for(auto symbol_data : reel_data) {
            if (symbol_data.is_string()) {
                // Direct Symbol
                string id = symbol_data.get<string>();
                if (!symbol_handler.hasSymbol(id)) {
                    throw std::invalid_argument("Symbol '" + id + "' not defined in symbol table.");
                }

                reel.addSymbol(symbol_handler.getSymbol(id), 1);
                continue;
            } else if (symbol_data.is_object()) {
                string id = extractStr("id", symbol_data);
                uint repeat = extractInt("repeat", symbol_data);

                if (!symbol_handler.hasSymbol(id)) {
                    throw std::invalid_argument("Symbol '" + id + "' not defined in symbol table.");
                }

                reel.addSymbol(symbol_handler.getSymbol(id), repeat);
                continue;
            }
        
            throw std::invalid_argument("'reels' can only contain either a string or object, but found: " + to_string(symbol_data));
        }
    }
}
#pragma endregion

#pragma region Run Methods
std::vector<std::vector<Symbol>> ReelHandler::runSpin() {
    for(uint i = 0; i < reels.size(); i++) {
        reels[i].spin(rng);
    }

    auto results = getSpinResult();
    stats.addSymbolMass(results);
    stats.increaseCount(1);
    return results;
}
#pragma endregion

#pragma region Accessor Methods
std::vector<std::vector<Symbol>> ReelHandler::getSpinResult() {
    auto ret = std::vector<std::vector<Symbol>>(reels.size(), std::vector<Symbol>());

    for(uint i = 0; i < reels.size(); i++) {
        auto& reel = reels[i];
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
    return reels[reelNum].getReelLength();
}
uint ReelHandler::getMaxReelLength() const {
    uint ret = 0;
    for(uint i = 0; i < getReelCount(); i++) {
        uint temp = getReelLength(i);
        ret = ret < temp ? ret : temp;
    }
    return ret;
}

StatsHandler ReelHandler::getStats() const {
    return stats;
}
#pragma endregion

#pragma region Helper
void ReelHandler::seedFromHardware() {
}
void ReelHandler::clear() {
    payoutRows = 0;
    stats.clear();
    reels.clear();
}
#pragma endregion