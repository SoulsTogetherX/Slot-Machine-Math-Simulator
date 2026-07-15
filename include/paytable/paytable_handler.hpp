#ifndef PAY_TABLE_HANDLER_HPP
#define PAY_TABLE_HANDLER_HPP

#include <vector>
#include <unordered_map>
#include <memory>
#include <nlohmann/json.hpp>

#include "utils/types.hpp"
#include "utils/symbol_types.hpp"
#include "symbol/symbol_handler.hpp"
#include "pattern/pattern_handler.hpp"
#include "paytable/paytable.hpp"



// The class used to store and evaluate PayTables, returning wins and payouts
// from the slotmachine.
class PayTableHandler {
private:
    // A map of all registered PayTables (id, PayTable)
    // Stored in pointer form as PayTable is an abstract class
    std::unordered_map<std::string, std::unique_ptr<PayTable>> paytables;

    // Extracts PayTables from nlohmann::json.
    void extractPayTables(
        const nlohmann::json& info, const PatternHandler& pattern_handler
    );
    // Extracts an optional per-paytable list of PayoutVariant rules.
    std::vector<PayoutVariant> extractVariants(const nlohmann::json& it);
public:
    // Extracts PayTables from nlohmann::json.
    void loadJson(
        const nlohmann::json& info, const PatternHandler& pattern_handler
    );

    bool hasPayTable(const std::string& id) const;
    PayTable& getPayTable(const std::string& id) const;

    // Returns non-owning views of every stored PayTable.
    std::vector<const PayTable*> getAllPayTables() const;
    // Returns the unique set of pattern ids referenced by the stored PayTables.
    std::vector<std::string> getReferencedPatternIds() const;

    // Aggergate the accumulated stats from all patterns
    StatsHandler aggergateStats() const;
    // Gets stats directly from a Paytable with given id
    StatsHandler getDirectStats(const std::string& id) const;

    // Evaluates every PayTable against its pattern's pre-computed convolutions.
    std::vector<PayoutResult> evaluateAll(const ConvolutionCache& cache);

    // Clears PayTables
    void clear();
};

#endif  // PAY_TABLE_HPP
