#include <sstream>

#include "pattern/custom_patterns/line_pattern.hpp"



#pragma region Line Pattern
template <typename F>
std::vector<SymbolLine> LinePattern::convolution_helper(
    F lambda, std::vector<SymbolLine> &ret, uint max
) const {
    if (lines.empty()) {
        ret.reserve(max);
        for(uint line = 0; line < max; line++) {
            lambda(line);
        }
        return ret;
    }
 
    ret.reserve(lines.size());
    for(uint line : lines) {
        lambda(line);
    }
    return ret;
}

std::string LinePattern::describe() const {
    std::ostringstream out;
    out << typeName() << " ";

    if (lines.empty()) {
        out << "(all)";
    } else {
        out << "[";
        for (size_t i = 0; i < lines.size(); i++) {
            if (i > 0) {
                out << ", ";
            }
            out << lines[i];
        }
        out << "]";
    }

    return out.str();
}
#pragma endregion

#pragma region Row Pattern
// Builds a SymbolLine per given row index by taking that row's symbol from every reel
std::vector<SymbolLine> RowPattern::convolution(
    const SymbolGrid &results
) const {
    auto ret = std::vector<SymbolLine>();
    const auto add_line = [&](uint row) {
        SymbolLine line;

        line.reserve(results.size());
        for(uint i = 0; i < results.size(); i++) {
            line.push_back(results[i][row]);
        }

        ret.push_back(std::move(line));
    };

    return convolution_helper(add_line, ret, results[0].size());
}
#pragma endregion

#pragma region Column Pattern
// Builds a SymbolLine per given column index by taking every symbol from that column reel
std::vector<SymbolLine> ColumnPattern::convolution(
    const SymbolGrid &results
) const {
    auto ret = std::vector<SymbolLine>();
    const auto add_line = [&](uint col) {
        SymbolLine line;

        line.reserve(results[col].size());
        for(uint i = 0; i < results[col].size(); i++) {
            line.push_back(results[col][i]);
        }

        ret.push_back(std::move(line));
    };

    return convolution_helper(add_line, ret, results.size());
}
#pragma endregion

#pragma region Diagonal Pattern
// Builds a SymbolLine per given diagonal index by taking a symbol from relevant reels
std::vector<SymbolLine> DiagonalPattern::convolution(
    const SymbolGrid &results
) const {
    auto ret = std::vector<SymbolLine>();
    const auto add_line = [&](uint idx) {
        const uint rows = results.size();
        const uint cols = results[0].size();
        SymbolLine line;

        for (uint i = 0; i < rows; ++i) {
            uint col = i + idx;
            
            if (col >= 0 && col < cols) {
                line.push_back(results[i][col]);
            }
        }
        ret.push_back(std::move(line));
    };

    return convolution_helper(add_line, ret, results.size());
}
#pragma endregion

#pragma region Diagonal Reversed Pattern
// Builds a SymbolLine per given reversed diagonal index by taking a symbol from relevant reels
std::vector<SymbolLine> DiagonalReversedPattern::convolution(
    const SymbolGrid &results
) const {
    auto ret = std::vector<SymbolLine>();
    const auto add_line = [&](uint idx) {
        const uint rows = results.size();
        const int cols = results[0].size();
        SymbolLine line;

        for (uint i = 0; i < rows; ++i) {
            int col = idx - 1 - i;
            
            if (col >= 0 && col < cols) {
                line.push_back(results[i][col]);
            }
        }
        ret.push_back(std::move(line));
    };

    return convolution_helper(add_line, ret, results.size());
}
#pragma endregion