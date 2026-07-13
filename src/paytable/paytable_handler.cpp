#include "paytable/paytable_handler.hpp"
#include "utilts/defs.hpp"
#include "utilts/extracts.hpp"



#pragma region Extract Info Helpers
void PayTableHandler::loadJson(
    const json& info, const PatternHandler& pattern_handler
) {
    extractPayTables(info.at("paytables"), pattern_handler);
}
void PayTableHandler::extractPayTables(
    const json& info, const PatternHandler& pattern_handler
) {
    if (!info.is_array()) {
        throw std::invalid_argument("Paytables is expected to be array, but found: " + to_string(info));
    }

    for(auto it : info) {
        string id = extractStr("id", it);
        if (hasPayTable(id)) {
            throw std::invalid_argument("PayTable id '" + id + "' has already been defined.");
        }

        string pattern_id = extractStr("pattern_id", it);
        if (!pattern_handler.hasPattern(pattern_id)) {
            throw std::invalid_argument("Pattern id '" + pattern_id + "' has not been defined.");
        }

        int payout = extractInt("payoutWin", it);
        int payoutFail = extractInt("payoutFail", it, 0);
        string symbol_requirement = extractStr("symbol_requirement", it);
        
        if (symbol_requirement == "matching") {
            paytables[id] = std::make_unique<PayTableMatching>(id, pattern_id, payout, payoutFail);
            continue;
        } else {
            throw std::invalid_argument("Paytable's 'symbol_requirement' is an invaild value.");
        }
    }
}
#pragma endregion

#pragma region Access Methods
bool PayTableHandler::hasPayTable(string id) const {
    return paytables.find(id) != paytables.end();
}
PayTable& PayTableHandler::getPayTable(string id) const {
    return *paytables.at(id);
}
#pragma endregion

#pragma region Stats
StatsHandler PayTableHandler::aggergateStats() const {
    StatsHandler ret;
    for (const auto& [key, value] : paytables) {
        ret.merge((*value).getStats());
    }
    return ret;
}
#pragma endregion

#pragma region Run Methods
std::vector<PayoutResult> PayTableHandler::evaluateAll(
    const std::vector<std::vector<Symbol>> &results,
    const PatternHandler& pattern_handler
) {
    auto ret = std::vector<PayoutResult>();
    ret.reserve(paytables.size());

    for(const auto& paytable : paytables) {
        auto convolutions = pattern_handler.getPattern(
            paytable.second->getPatternId()
        ).convolution(results);

        for(auto convolution : convolutions) {
            ret.push_back(paytable.second->evaluate(convolution));
        }
    }

    return ret;
}
#pragma endregion

#pragma region Helpers
void PayTableHandler::clear() {
    paytables.clear();
}
#pragma endregion