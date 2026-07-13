#include "paytable/paytable.hpp"
#include "utilts/defs.hpp"
#include "utilts/spinResult.hpp"


#pragma region Helper
string PayTable::getId() const {
    return id;
}
string PayTable::getPatternId() const {
    return pattern_id;
}
#pragma endregion


#pragma region PayTable
#include <iostream>
SpinResult PayTable::evaluate(const std::vector<Symbol>& result) const {
    SpinResult ret;
    ret.payoutWon = isVaildMatch(result);

    if (ret.payoutWon) {
        ret.payout = 0;
    } else {
        ret.payout = 0;
    }

    ret.pattern_id = getPatternId();
    ret.payout_id = getId();
    ret.symbols = result;

    std::cout << ret.pattern_id << std::endl;
    std::cout << ret.payout_id << std::endl;
    std::cout << ret.payout << std::endl;
    std::cout << ret.payoutWon << std::endl;
    std::cout << "[";
    for(auto sym : ret.symbols) {
        std::cout << sym.getId() << ", ";
    }
    std::cout << "]" << std::endl;

    return ret;
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