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
            if (hasKey("variants", it) && hasKey("payoutWin", it)) {
                throw std::invalid_argument(
                    "PayTable '" + id + "' cannot specify both 'variants' and a flat 'payoutWin'; use one or the other."
                );
            }

            if (hasKey("variants", it)) {
                std::vector<PayoutVariant> variants = extractVariants(it);
                paytables[id] = std::make_unique<PayTableMatching>(
                    id, pattern_id, std::move(variants), payoutFail
                );
            } else {
                int payout = extractInt("payoutWin", it);
                paytables[id] = std::make_unique<PayTableMatching>(
                    id, pattern_id, payout, payoutFail
                );
            }
            continue;
        } else {
            throw std::invalid_argument("Paytable's 'symbol_requirement' is an invaild value.");
        }
    }
}

std::vector<PayoutVariant> PayTableHandler::extractVariants(const nlohmann::json& it) {
    const nlohmann::json& arr = it.at("variants");
    if (!arr.is_array()) {
        throw std::invalid_argument("'variants' is expected to be an array, but found: " + to_string(arr));
    }

    std::vector<PayoutVariant> variants;
    variants.reserve(arr.size());

    for (const auto& v : arr) {
        PayoutVariant variant;

        if (hasKey("symbols", v)) {
            const nlohmann::json& syms = v.at("symbols");
            if (!syms.is_array()) {
                throw std::invalid_argument("Variant's 'symbols' is expected to be an array, but found: " + to_string(syms));
            }
            for (const auto& s : syms) {
                if (!s.is_string()) {
                    throw std::invalid_argument("Variant's 'symbols' array must contain strings.");
                }
                variant.symbols.push_back(s.get<std::string>());
            }
        }

        // An empty/omitted 'symbols' list is a valid catch-all.
        variant.withWilds = extractBool("withWilds", v, true);
        variant.payoutWin = extractInt("payoutWin", v);
        variant.payoutFail = extractInt("payoutFail", v, 0);

        variants.push_back(std::move(variant));
    }

    return variants;
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
// Dedupes so a pattern shared by multiple paytables is only convolved once per spin.
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