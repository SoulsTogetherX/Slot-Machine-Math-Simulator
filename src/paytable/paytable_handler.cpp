#include <unordered_set>
#include <nlohmann/json.hpp>

#include "paytable/paytable_handler.hpp"
#include "utils/types.hpp"
#include "utils/extracts.hpp"



#pragma region Extract Info Helpers
void PayTableHandler::loadJson(
    const nlohmann::json& info, const PatternHandler& pattern_handler
) {
    extractPayTables(info.at("paytables"), pattern_handler);
}
void PayTableHandler::extractPayTables(
    const nlohmann::json& info, const PatternHandler& pattern_handler
) {
    if (!info.is_array()) {
        throw std::invalid_argument("Paytables is expected to be array, but found: " + to_string(info));
    }

    for(const auto& it : info) {
        std::string id = extractStr("id", it);
        if (hasPayTable(id)) {
            throw std::invalid_argument("PayTable id '" + id + "' has already been defined.");
        }

        std::string pattern_id = extractStr("pattern_id", it);
        if (!pattern_handler.hasPattern(pattern_id)) {
            throw std::invalid_argument("Pattern id '" + pattern_id + "' has not been defined.");
        }

        int payoutFail = extractInt("payoutFail", it, 0);
        std::string symbol_requirement = extractStr("symbol_requirement", it);

        if (symbol_requirement == "matching") {
            int payout = extractInt("payoutWin", it);
            paytables[id] = std::make_unique<PayTableMatching>(
                id, pattern_id, payout, payoutFail
            );
            continue;
        } else {
            throw std::invalid_argument("Paytable's 'symbol_requirement' is an invaild value.");
        }
    }
}
#pragma endregion

#pragma region Access Methods
bool PayTableHandler::hasPayTable(const std::string& id) const {
    return paytables.find(id) != paytables.end();
}
PayTable& PayTableHandler::getPayTable(const std::string& id) const {
    return *paytables.at(id);
}
std::vector<const PayTable*> PayTableHandler::getAllPayTables() const {
    auto ret = std::vector<const PayTable*>();
    ret.reserve(paytables.size());

    for(const auto& [key, value] : paytables) {
        ret.push_back(value.get());
    }
    return ret;
}
// Dedupes so a pattern shared by multiple paytables is only convolved once per spin
// (see DESIGN.md item 1: this is what avoids double-counting a pattern's stats).
std::vector<std::string> PayTableHandler::getReferencedPatternIds() const {
    std::unordered_set<std::string> seen;
    std::vector<std::string> ret;
    ret.reserve(paytables.size());

    for(const auto& [key, value] : paytables) {
        if (seen.insert(value->getPatternId()).second) {
            ret.push_back(value->getPatternId());
        }
    }
    return ret;
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
StatsHandler PayTableHandler::getDirectStats(const std::string& id) const {
    return paytables.at(id)->getStats();
}
#pragma endregion

#pragma region Run Methods
// Scores every paytable against its pattern's already-computed convolutions from
// 'cache', producing one PayoutResult per convolution (e.g. one per payline).
std::vector<PayoutResult> PayTableHandler::evaluateAll(const ConvolutionCache& cache) {
    auto ret = std::vector<PayoutResult>();
    ret.reserve(paytables.size());

    for(const auto& [key, paytable] : paytables) {
        const std::vector<SymbolLine>& convolutions = cache.at(paytable->getPatternId());

        for(const SymbolLine& convolution : convolutions) {
            ret.push_back(paytable->evaluate(convolution));
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