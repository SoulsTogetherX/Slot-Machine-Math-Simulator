#ifndef PAY_TABLE_HANDLER_HPP
#define PAY_TABLE_HANDLER_HPP

#include <unordered_map>
#include <memory>

#include "utilts/defs.hpp"
#include "utilts/spinResult.hpp"
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
    const PayTable& getPayTable(string id) const;

    std::vector<SpinResult> evaluateAll(
        const std::vector<std::vector<Symbol>> &results, const PatternHandler& pattern_handler
    ) const;

    void clear();
};

#endif  // PAY_TABLE_HPP
