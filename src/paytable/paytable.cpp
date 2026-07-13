#include "paytable/paytable.hpp"
#include "utilts/defs.hpp"



#pragma region Access Methods
string PayTable::getId() const {
    return id;
}
string PayTable::getPatternId() const {
    return pattern_id;
}

StatsHandler PayTable::getStats() const {
    return stats;
}
#pragma endregion

#pragma region Run Methods
PayoutResult PayTable::evaluate(const std::vector<Symbol>& result) {
    PayoutResult payout;
    if (isVaildMatch(result)) {
        stats.addSymbols(result);
        stats.increaseCount(1);

        payout.won = true;
        payout.payout = 1;
    } else {
        payout.won = false;
        payout.payout = 0;
    }
    return payout;
}
#pragma endregion

#pragma region PayTable Matching
bool PayTableMatching::isVaildMatch(const std::vector<Symbol>& result) const {
    if (result.empty()) {
        return false;
    }

    uint idx = 0;
    Symbol base;

    for(; idx < result.size(); idx++) {
        if (result[idx].getType() != WILD) {
            base = result[idx];
            break;
        }
    }
    for(; idx < result.size(); idx++) {
        if (!result[idx].matches(base)) {
            return false;
        }
    }

    return true;
}
#pragma endregion