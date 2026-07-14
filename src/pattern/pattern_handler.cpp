#include "pattern/pattern_handler.hpp"
#include "utilts/defs.hpp"
#include "utilts/extracts.hpp"



#pragma region Extract Info Helpers
void PatternHandler::loadJson(const json& info, const ReelHandler& reel_handler) {
    extractPatterns(info.at("patterns"), reel_handler);
}
void PatternHandler::extractPatterns(const json& info, const ReelHandler& reel_handler) {
    if (!info.is_array()) {
        throw std::invalid_argument("Patterns is expected to be array, but found: " + to_string(info));
    }

    for(const auto& it : info) {
        string id = extractStr("id", it);
        if (hasPattern(id)) {
            throw std::invalid_argument("Pattern id '" + id + "' has already been defined.");
        }

        string match_type = extractStr("match_type", it);
        if (match_type == "line") {
            uint row = extractInt("row", it);
            if (0 >= row || row > reel_handler.getPayoutRows()) {
                throw std::range_error("Pattern '" + match_type + "' type has an invaild row.");
                return;
            }

            patterns[id] = std::make_unique<LinePattern>(id, row);
            continue;
        } else {
            throw std::invalid_argument("Pattern's '" + match_type + "' is an invaild value.");
        }
    }
}
#pragma endregion

#pragma region Run Methods
ConvolutionCache PatternHandler::convolveAll(
    const SymbolGrid& screen, const std::vector<string>& pattern_ids
) {
    ConvolutionCache cache;
    cache.reserve(pattern_ids.size());

    for (const string& id : pattern_ids) {
        cache[id] = patterns.at(id)->convolution(screen);
    }
    return cache;
}
#pragma endregion

#pragma region Accessor Methods
bool PatternHandler::hasPattern(const string& id) const {
    return patterns.find(id) != patterns.end();
}
Pattern& PatternHandler::getPattern(const string& id) const {
    return *patterns.at(id);
}
std::vector<const Pattern*> PatternHandler::getAllPatterns() const {
    auto ret = std::vector<const Pattern*>();
    ret.reserve(patterns.size());

    for(const auto& [key, value] : patterns) {
        ret.push_back(value.get());
    }
    return ret;
}
#pragma endregion

#pragma region Stats
StatsHandler PatternHandler::aggergateStats() const {
    StatsHandler ret;
    for (const auto& [key, value] : patterns) {
        ret.merge((*value).getStats());
    }
    return ret;
}
StatsHandler PatternHandler::getDirectStats(const string& id) const {
    return patterns.at(id)->getStats();
}
#pragma endregion

#pragma region Helpers
void PatternHandler::clear() {
    patterns.clear();
}
#pragma endregion