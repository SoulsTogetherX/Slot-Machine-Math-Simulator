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
PayoutResult PayTable::evaluate(const SymbolLine& result) {
    PayoutResult payout_result;
    if (isVaildMatch(result)) {
        stats.addSymbols(result);
        stats.increaseCount(1);

        payout_result.won = true;
        payout_result.payout = payoutWin;
    } else {
        payout_result.won = false;
        payout_result.payout = payoutFail;
    }
    return payout_result;
}
#pragma endregion

#pragma region PayTable Matching
bool PayTableMatching::isVaildMatch(const SymbolLine& result) const {
    if (result.empty()) {
        return false;
    }

    uint idx = 0;
    const Symbol* base = nullptr;

    for(; idx < result.size(); idx++) {
        if (result[idx]->getType() != WILD) {
            base = result[idx];
            break;
        }
    }

    // Every symbol was WILD, so the line trivially matches.
    if (base == nullptr) {
        return true;
    }

    for(; idx < result.size(); idx++) {
        if (!result[idx]->matches(*base)) {
            return false;
        }
    }

    return true;
}
#pragma endregion