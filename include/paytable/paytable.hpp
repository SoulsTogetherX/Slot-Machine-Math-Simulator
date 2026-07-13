#ifndef PAY_TABLE_HPP
#define PAY_TABLE_HPP

#include "utilts/defs.hpp"
#include "utilts/payout_result.hpp"
#include "stats/stats_handler.hpp"
#include "symbol/symbol.hpp"



class PayTable {
private:
    StatsHandler stats;

    string id, pattern_id;

    virtual bool isVaildMatch(const std::vector<Symbol>& result) const = 0;
public:
    PayTable() {};
    PayTable(string id, string pattern_id) : id(id), pattern_id(pattern_id) {};

    string getId() const;
    string getPatternId() const;
    StatsHandler getStats() const;

    PayoutResult evaluate(const std::vector<Symbol>& result);
};

class PayTableMatching : public PayTable {
private:
    bool isVaildMatch(const std::vector<Symbol>& result) const;
public:
    PayTableMatching() {};
    PayTableMatching(string id, string pattern_id) : PayTable(id, pattern_id) {};
};

#endif  // PAY_TABLE_HPP
