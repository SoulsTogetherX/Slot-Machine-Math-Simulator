#include "pattern/pattern.hpp"


#pragma region Accessors
string Pattern::getId() const {
    return id;
}
#pragma endregion

#pragma region Line Pattern
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