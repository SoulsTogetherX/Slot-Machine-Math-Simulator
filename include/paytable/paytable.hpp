#ifndef PAY_TABLE_HPP
#define PAY_TABLE_HPP

#include "utilts/defs.hpp"
#include "utilts/payout_result.hpp"
#include "stats/stats_handler.hpp"
#include "symbol/symbol.hpp"



// The abstract class responsible for checking if a given a convolution is vaild,
// then calculating/return the win of a slot machine (if vaild).
// Overwrite 'isVaildMatch' to handle different symbol checks types.
class PayTable {
private:
    StatsHandler stats;

    string id; // The PayTable's name
    string pattern_id; // The attached Pattern's name

    int payoutWin; // The payout gained from succeeding
    int payoutFail; // The payout gained from failing

    // Returns if the given convolution is considered a vaild win.
    virtual bool isVaildMatch(const std::vector<Symbol>& result) const = 0;
public:
    PayTable() {};
    PayTable(
        string id, string pattern_id, int payoutWin, int payoutFail
    ) : id(id), pattern_id(pattern_id), payoutWin(payoutWin), payoutFail(payoutFail) {};

    // Accessor Methods
    string getId() const;
    string getPatternId() const;
    StatsHandler getStats() const;

    // Returns the result of a convolution (payout and if a vaild win), while recording
    // the sstats.
    PayoutResult evaluate(const std::vector<Symbol>& result);
};

class PayTableMatching : public PayTable {
private:
    bool isVaildMatch(const std::vector<Symbol>& result) const override;
public:
    PayTableMatching() {};
    PayTableMatching(
        string id, string pattern_id, int payoutWin, int payoutFail
    ) : PayTable(id, pattern_id, payoutWin, payoutFail) {};
};

#endif  // PAY_TABLE_HPP
