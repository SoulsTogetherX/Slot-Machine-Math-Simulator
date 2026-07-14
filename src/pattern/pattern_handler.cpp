#include "pattern/pattern_handler.hpp"
#include "utilts/defs.hpp"
#include "utilts/extracts.hpp"



#pragma region Extract Info Helpers
static std::vector<uint> extractRows(const json& it, uint payout_rows) {
    if (!it.contains("row") || it.at("row").is_null()) {
        throw std::out_of_range("Line pattern requires a 'row'.");
    }

    const json& row_info = it.at("row");
    std::vector<uint> rows;

    auto add_row = [&](int r) {
        if (r <= 0 || static_cast<uint>(r) > payout_rows) {
            throw std::range_error("Line pattern has an invalid row: " + std::to_string(r));
        }
        rows.push_back(static_cast<uint>(r));
    };

    if (row_info.is_number_integer()) {
        add_row(row_info.get<int>());
    } else if (row_info.is_array()) {
        if (row_info.empty()) {
            throw std::invalid_argument("Line pattern 'row' array must not be empty.");
        }
        for (const auto& r : row_info) {
            if (!r.is_number_integer()) {
                throw std::invalid_argument("Line pattern 'row' array must contain integers.");
            }
            add_row(r.get<int>());
        }
    } else {
        throw std::invalid_argument("Line pattern 'row' must be an integer or an array of integers.");
    }

    return rows;
}

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
            std::vector<uint> rows = extractRows(it, reel_handler.getPayoutRows());
            patterns[id] = std::make_unique<LinePattern>(id, rows);
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

#pragma region Helpers
void PatternHandler::clear() {
    patterns.clear();
}
#pragma endregion