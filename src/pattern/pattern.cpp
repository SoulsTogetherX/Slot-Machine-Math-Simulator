#include "pattern/pattern.hpp"


#pragma region Accessors
std::string Pattern::getId() const {
    return id;
}
#pragma endregion

#pragma region Line Pattern
// Builds one SymbolLine per configured row by taking that row's symbol from every
// reel; 'rows' are stored 1-indexed (as configured), hence the 'row - 1' offset.
std::vector<SymbolLine> LinePattern::convolution(
    const SymbolGrid &results
) const {
    auto ret = std::vector<SymbolLine>();
    ret.reserve(rows.size());

    for(uint row : rows) {
        SymbolLine line;
        line.reserve(results.size());
        for(uint i = 0; i < results.size(); i++) {
            line.push_back(results[i][row - 1]);
        }
        ret.push_back(std::move(line));
    }

    return ret;
}
#pragma endregion