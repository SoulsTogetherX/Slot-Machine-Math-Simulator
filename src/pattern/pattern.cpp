#include "pattern/pattern.hpp"


#pragma region Accessors
string Pattern::getId() const {
    return id;
}
StatsHandler Pattern::getStats() const {
    return stats;
}
#pragma endregion

#pragma region Pattern Convolution
std::vector<std::vector<Symbol>> Pattern::convolution(
    const std::vector<std::vector<Symbol>> &results 
) {
    auto ret = handleConvolution(results);
    stats.addSymbolMass(ret);
    stats.increaseCount(ret.size());
    return ret;
}
#pragma endregion

#pragma region Line Pattern
std::vector<std::vector<Symbol>> LinePattern::handleConvolution(
    const std::vector<std::vector<Symbol>> &results 
) const {
    auto ret = std::vector<std::vector<Symbol>>();
    ret.resize(1);
    ret[0].reserve(results.size());

    for(uint i = 0; i < results.size(); i++) {
        ret[0].push_back(results[i][row - 1]);
    }

    return ret;
}
#pragma endregion