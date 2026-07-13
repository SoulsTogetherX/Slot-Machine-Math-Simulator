#include "pattern/pattern.hpp"


string Pattern::getId() const {
    return id;
}


#pragma region Line Pattern
LinePattern::LinePattern() : Pattern(), row(0) {};
LinePattern::LinePattern(string id, uint row) : Pattern(id), row(row) {};


std::vector<std::vector<Symbol>> LinePattern::convolution(
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