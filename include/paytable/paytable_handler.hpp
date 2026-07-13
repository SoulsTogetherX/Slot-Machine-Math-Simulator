#ifndef PAY_TABLE_HANDLER_HPP
#define PAY_TABLE_HANDLER_HPP

#include <unordered_map>
#include <memory>

#include "utilts/defs.hpp"
#include "utilts/payout_result.hpp"
#include "symbol/symbol_handler.hpp"
#include "pattern/pattern_handler.hpp"
#include "paytable/paytable.hpp"



// The class used to store and evaluate PayTables, returning wins and payouts
// from the slotmachine.
class PayTableHandler {
private:
    // A map of all registered PayTables (id, PayTable)
    // Stored in pointer form as PayTable is an abstract class
    std::unordered_map<string, std::unique_ptr<PayTable>> paytables;

    // Extracts PayTables from json.
    void extractPayTables(
        const json& info, const PatternHandler& pattern_handler
    );
public:
    // Extracts PayTables from json.
    void loadJson(
        const json& info, const PatternHandler& pattern_handler
    );

    // Accessor Methods
    bool hasPayTable(string id) const;
    PayTable& getPayTable(string id) const;

    // Aggergate the accumulated stats from all patterns
    StatsHandler aggergateStats() const;

    // Returns a vector of all PayoutResult from all stored PayTables
    std::vector<PayoutResult> evaluateAll(
        const std::vector<std::vector<Symbol>> &results, const PatternHandler& pattern_handler
    );

    void clear();
};

#endif  // PAY_TABLE_HPP
