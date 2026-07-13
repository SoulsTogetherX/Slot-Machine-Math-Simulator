#ifndef PAY_TABLE_HANDLER_HPP
#define PAY_TABLE_HANDLER_HPP

#include <unordered_map>
#include <memory>

#include "utilts/defs.hpp"
#include "utilts/payout_result.hpp"
#include "symbol/symbol_handler.hpp"
#include "pattern/pattern_handler.hpp"
#include "paytable/paytable.hpp"



class PayTableHandler {
private:
    std::unordered_map<string, std::unique_ptr<PayTable>> paytables;

    void extractPayTables(
        const json& info, const PatternHandler& pattern_handler
    );
public:
    void loadJson(
        const json& info, const PatternHandler& pattern_handler
    );

    bool hasPayTable(string id) const;
    PayTable& getPayTable(string id) const;

    StatsHandler aggergateStats() const;
    std::vector<PayoutResult> evaluateAll(
        const std::vector<std::vector<Symbol>> &results, const PatternHandler& pattern_handler
    );

    void clear();
};

#endif  // PAY_TABLE_HPP
