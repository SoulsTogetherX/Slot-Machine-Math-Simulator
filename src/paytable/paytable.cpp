#include "paytable/paytable.hpp"
#include "utilts/defs.hpp"



#pragma region Access Methods
string PayTable::getId() const {
    return id;
}
string PayTable::getPatternId() const {
    return pattern_id;
}

const StatsHandler& PayTable::getStats() const {
    return stats;
}
#pragma endregion

#pragma region Run Methods
PayoutResult PayTable::evaluate(const SymbolLine& line) {
    bool won = false;
    int payout = scoreLine(line, won);

    if (won) {
        int multiplier = 1;
        for (const Symbol* s : line) {
            multiplier *= s->getMultiplier();
        }
        payout *= multiplier;

        stats.addSymbols(line);
        stats.increaseCount(1);
    }

    PayoutResult result;
    result.won = won;
    result.payout = payout;
    return result;
}
#pragma endregion

#pragma region PayTable Matching
bool PayTableMatching::isMatch(const SymbolLine& line) const {
    if (line.empty()) {
        return false;
    }

    uint idx = 0;
    const Symbol* base = nullptr;

    for(; idx < line.size(); idx++) {
        if (line[idx]->getType() != WILD) {
            base = line[idx];
            break;
        }
    }
    if (base == nullptr) {
        return true;
    }

    for(; idx < line.size(); idx++) {
        if (!line[idx]->matches(*base)) {
            return false;
        }
    }
    return true;
}
int PayTableMatching::scoreLine(const SymbolLine& line, bool& won) const {
    if (isMatch(line)) {
        won = true;
        return payoutWin;
    }
    won = false;
    return payoutFail;
}
#pragma endregion
