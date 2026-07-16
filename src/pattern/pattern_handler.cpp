#include <nlohmann/json.hpp>

#include "pattern/pattern_handler.hpp"
#include "pattern/custom_patterns/line_pattern.hpp"
#include "utils/types.hpp"
#include "utils/extracts.hpp"



#pragma region Extract Info Helpers
void PatternHandler::loadJson(const nlohmann::json& info, const ReelHandler& reel_handler) {
    extractPatterns(info.at("patterns"), reel_handler);
}
void PatternHandler::extractPatterns(const nlohmann::json& info, const ReelHandler& reel_handler) {
    if (!info.is_array()) {
        throw std::invalid_argument("Patterns is expected to be array, but found: " + to_string(info));
    }

    for(const auto& it : info) {
        const std::string id = extractStr("id", it);
        if (hasPattern(id)) {
            throw std::invalid_argument("Pattern id '" + id + "' has already been defined.");
        }

        const std::string match_type = extractStr("match_type", it);
        if (match_type == "line") {
            const std::string line_type = extractStr("lineType", it);
            if (line_type == "row") {
                std::vector<uint> indexes = extractIndexes(
                    it,
                    // Number of rows
                    reel_handler.getPayoutRows()
                );
                patterns[id] = std::make_unique<RowPattern>(id, indexes);
            } else if (line_type == "col") {
                std::vector<uint> indexes = extractIndexes(
                    it,
                    // Number of columns
                    reel_handler.getReelCount()
                );
                patterns[id] = std::make_unique<ColumnPattern>(id, indexes);
            } else if (line_type == "diagonal") {
                std::vector<uint> indexes = extractIndexes(
                    it,
                    // Number of rows + columns - 1
                    reel_handler.getPayoutRows() + reel_handler.getReelCount() - 1
                );
                patterns[id] = std::make_unique<DiagonalPattern>(id, indexes);
            } else if (line_type == "diagonalReversed") {
                std::vector<uint> indexes = extractIndexes(
                    it,
                    // Number of rows + columns - 1
                    reel_handler.getPayoutRows() + reel_handler.getReelCount() - 1
                );
                patterns[id] = std::make_unique<DiagonalReversedPattern>(id, indexes);
            } else {
                throw std::invalid_argument("Unknown 'lineType' found: " + line_type);
            }
            continue;
        } else {
            throw std::invalid_argument("Pattern's '" + match_type + "' is an invaild value.");
        }
    }
}

std::vector<uint> PatternHandler::extractIndexes(const nlohmann::json& info, uint maxIndex) {
    if (!hasKey("index", info)) {
        return std::vector<uint>();
    }

    const nlohmann::json& line_info = info.at("index");
    std::vector<uint> lines;

    auto add_line = [&](int idx) {
        if (idx < 0 || static_cast<uint>(idx) >= maxIndex) {
            throw std::range_error("Line pattern has an invalid index: " + std::to_string(idx));
        } else if (std::find(lines.begin(), lines.end(), idx) != lines.end()) {
            throw std::range_error("Line pattern has a duplicate index: " + std::to_string(idx));
        }
        lines.push_back(static_cast<uint>(idx));
    };

    if (line_info.is_number_integer()) {
        add_line(line_info.get<int>());
    } else if (line_info.is_array()) {
        if (line_info.empty()) {
            throw std::invalid_argument("Line pattern 'index' array must not be empty.");
        }
        for (const auto& line : line_info) {
            if (!line.is_number_integer()) {
                throw std::invalid_argument("Line pattern 'index' array must contain only integer indexes.");
            }
            add_line(line.get<int>());
        }
    } else {
        throw std::invalid_argument("Line pattern 'index' must be an integer or an array of integers.");
    }

    return lines;
}
#pragma endregion

#pragma region Run Methods
ConvolutionCache PatternHandler::convolveAll(
    const SymbolGrid& screen, const std::vector<std::string>& pattern_ids
) {
    ConvolutionCache cache;
    cache.reserve(pattern_ids.size());

    for (const std::string& id : pattern_ids) {
        cache[id] = patterns.at(id)->convolution(screen);
    }
    return cache;
}
#pragma endregion

#pragma region Accessor Methods
bool PatternHandler::hasPattern(const std::string& id) const {
    return patterns.find(id) != patterns.end();
}
Pattern& PatternHandler::getPattern(const std::string& id) const {
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