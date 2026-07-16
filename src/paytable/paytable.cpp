#include <algorithm>

#include "paytable/paytable.hpp"
#include "utils/types.hpp"



#pragma region Helpers (File local)
namespace {
    // Shared recording logic used for both a PayTable's own stats and (when
    // applicable) the specific PayoutVariant that fired, so the two never
    // drift out of sync with each other.
    void recordOutcome(StatsHandler& stats, const SymbolLine& line, double payout, bool won) {
        if (won) {
            stats.addSymbols(line);
            stats.increaseCount(1);
            stats.addPayout(payout);
        } else if (payout != 0) {
            stats.increaseConsolationCount(1);
            stats.addConsolationPayout(payout);
        }
    }
}
#pragma endregion

#pragma region Access Methods
std::string PayTable::getId() const {
    return id;
}
std::string PayTable::getPatternId() const {
    return pattern_id;
}

const StatsHandler& PayTable::getStats() const {
    return stats;
}

void PayTable::registerSymbols(const SymbolHandler& symbol_handler) {
    stats.registerSymbols(symbol_handler);
}
#pragma endregion

#pragma region Run Methods
// Scores a given line.
PayoutResult PayTable::evaluate(const SymbolLine& line) {
    bool won = false;
    int payout = scoreLine(line, won);

    if (won) {
        int multiplier = 1;
        for (const Symbol* s : line) {
            multiplier *= s->getMultiplier();
        }
        payout *= multiplier;
    }

    recordOutcome(stats, line, payout, won);

    PayoutResult result;
    result.won = won;
    result.payout = payout;
    return result;
}
#pragma endregion

#pragma region PayTable Matching
// Finds the first non-wild symbol on the line. Returns nullptr if
// every position is WILD .
const Symbol* PayTableMatching::findBaseSymbol(const SymbolLine& line) const {
    for (const Symbol* s : line) {
        if (s->getType() != WILD) {
            return s;
        }
    }
    return nullptr;
}

// Checks whether every position on the line matches 'base'. 'withWilds'
// controls whether a WILD symbol may substitute.
bool PayTableMatching::lineMatchesSymbol(const SymbolLine& line, const Symbol& base, bool withWilds) const {
    for (const Symbol* s : line) {
        if (withWilds) {
            if (!s->matches(base)) {
                return false;
            }
        } else {
            if (s->getId() != base.getId()) {
                return false;
            }
        }
    }
    return true;
}

// A line matches if, ignoring wilds, every symbol is identical. The first non-wild
// symbol found sets the 'base' to match against.
// An all-wild line always matches.
bool PayTableMatching::isMatch(const SymbolLine& line) const {
    if (line.empty()) {
        return false;
    }

    const Symbol* base = findBaseSymbol(line);
    if (base == nullptr) {
        return true;
    }
    return lineMatchesSymbol(line, *base, true);
}

int PayTableMatching::scoreLine(const SymbolLine& line, bool& won, PayoutVariant** matchedVariant) {
    if (!variants.empty()) {
        const Symbol* base = findBaseSymbol(line);

        for (PayoutVariant& variant : variants) {
            if (
                base != nullptr && // No base was found, only wilds exist
                !variant.symbols.empty() && // If variants is empty, check for any symbol
                std::find(variant.symbols.begin(), variant.symbols.end(), base->getId()) == variant.symbols.end() // If the base must be found in variant's symbol filter to count
            ) {
                continue; // try the next variant
            }

            const bool lineMatches = (base == nullptr) ? true : lineMatchesSymbol(line, *base, variant.withWilds);
            won = lineMatches;
            if (matchedVariant != nullptr) {
                *matchedVariant = &variant;
            }
            return lineMatches ? variant.payoutWin : variant.payoutFail;
        }

        // No variant's symbol filter covers this line's base symbol at all.
        won = false;
        return payoutFail;
    }

    // Flat behavior: unchanged from before variants existed.
    if (isMatch(line)) {
        won = true;
        return payoutWin;
    }
    won = false;
    return payoutFail;
}

void PayTableMatching::registerSymbols(const SymbolHandler& symbol_handler) {
    PayTable::registerSymbols(symbol_handler);
    for (PayoutVariant& variant : variants) {
        variant.stats.registerSymbols(symbol_handler);
    }
}

PayoutResult PayTableMatching::evaluate(const SymbolLine& line) {
    bool won = false;
    PayoutVariant* matched = nullptr;
    int payout = scoreLine(line, won, &matched);

    if (won) {
        int multiplier = 1;
        for (const Symbol* s : line) {
            multiplier *= s->getMultiplier();
        }
        payout *= multiplier;
    }

    recordOutcome(stats, line, payout, won);
    if (matched != nullptr) {
        recordOutcome(matched->stats, line, payout, won);
    }

    PayoutResult result;
    result.won = won;
    result.payout = payout;
    return result;
}

bool PayTableMatching::hasVariants() const {
    return !variants.empty();
}
const std::vector<PayoutVariant>& PayTableMatching::getVariants() const {
    return variants;
}
#pragma endregion
