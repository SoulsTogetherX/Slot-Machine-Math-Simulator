#ifndef PAY_TABLE_HPP
#define PAY_TABLE_HPP

#include <vector>

#include "utils/types.hpp"
#include "utils/symbol_types.hpp"
#include "stats/stats_handler.hpp"
#include "symbol/symbol.hpp"
#include "symbol/symbol_handler.hpp"


// The result of scoring one convolution: the credits awarded, and whether it
// counts as a win (true) or a consolation payout (false).
struct PayoutResult {
    double payout;
    bool won;
};

// One entry of a PayTableMatching's optional per-symbol payout rules.
struct PayoutVariant {
    std::vector<std::string> symbols; // Eligible base symbol ids. Empty = any symbol.
    bool withWilds = true;            // Whether WILD may substitute when checking this variant's match.
    int payoutWin = 0;
    int payoutFail = 0;

    // Tracks this variant's own stats.
    StatsHandler stats;
};


// The abstract class responsible for scoring a convolution into a payout.
// Overwrite 'scoreLine' to handle different scoring schemes.
class PayTable {
private:
    std::string id;          // The PayTable's name
    std::string pattern_id;  // The attached Pattern's name
protected:
    StatsHandler stats;

    // An abstract method to return if the given Symbol convolution is a vaild match for this PayTable
    virtual bool isMatch(const SymbolLine& line) const = 0;
    // An abstract method to return the payout for this Symbol convolution. Subclasses
    // that track sub-groupings (e.g. PayTableMatching's variants) may report back
    // which sub-grouping fired via 'matchedVariant', so the caller can record stats
    // against it too; subclasses without sub-groupings simply ignore the parameter.
    virtual int scoreLine(const SymbolLine& line, bool& won, PayoutVariant** matchedVariant = nullptr) = 0;
public:
    PayTable() {};
    PayTable( std::string id, std::string pattern_id) : id(id), pattern_id(pattern_id) {};
    virtual ~PayTable() = default;

    std::string getId() const;
    std::string getPatternId() const;
    const StatsHandler& getStats() const;

    // Pre-seeds this PayTable's stats with every known symbol at a '0' count,
    // so symbols that never land on a winning line still show up in reports.
    // Virtual so subclasses with sub-groupings (variants) can also pre-seed those.
    virtual void registerSymbols(const SymbolHandler& symbol_handler);

    // Returns the result of a convolution (payout and whether it won), applying
    // per-line-bet scaling to wins when configured, while recording the stats.
    virtual PayoutResult evaluate(const SymbolLine& line);
};

// Scores a line as a win only when every (non-wild) symbol on it is identical,
// paying a flat 'payoutWin' (or 'payoutFail' otherwise).
class PayTableMatching : public PayTable {
private:
    int payoutWin;    // Used only when 'variants' is empty.
    int payoutFail;   // Payout when not won; also the fallback when no variant's
                      // symbol filter matches the line's base symbol at all.
    std::vector<PayoutVariant> variants;

    // Finds the line's first non-wild symbol ("base"), or nullptr if every
    // position on the line is WILD.
    const Symbol* findBaseSymbol(const SymbolLine& line) const;
    // Checks whether every position on the line matches 'base'. When
    // 'withWilds' is true, WILD symbols substitute freely.
    bool lineMatchesSymbol(const SymbolLine& line, const Symbol& base, bool withWilds) const;

    bool isMatch(const SymbolLine& line) const override;
    int scoreLine(const SymbolLine& line, bool& won, PayoutVariant** matchedVariant = nullptr) override;
public:
    PayTableMatching() {};
    PayTableMatching(
        std::string id, std::string pattern_id, int payoutWin, int payoutFail
    ) : PayTable(id, pattern_id), payoutWin(payoutWin), payoutFail(payoutFail) {};
    PayTableMatching(
        std::string id, std::string pattern_id, std::vector<PayoutVariant> variants, int payoutFail
    ) : PayTable(id, pattern_id), payoutWin(0), payoutFail(payoutFail), variants(std::move(variants)) {};

    // Also pre-seeds every variant's own stats.
    void registerSymbols(const SymbolHandler& symbol_handler) override;
    // Applies the win multiplier as before, then records stats against both
    // this PayTable and (if one fired) the specific PayoutVariant involved.
    PayoutResult evaluate(const SymbolLine& line) override;

    // Whether this paytable uses per-symbol variants rather than a flat payout.
    bool hasVariants() const;
    // Non-owning view of the configured variants, in declaration order.
    const std::vector<PayoutVariant>& getVariants() const;
};

#endif  // PAY_TABLE_HPP
