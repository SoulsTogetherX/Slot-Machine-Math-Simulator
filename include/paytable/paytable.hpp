#ifndef PAY_TABLE_HPP
#define PAY_TABLE_HPP

#include "utilts/defs.hpp"
#include "utilts/spinResult.hpp"
#include "symbol/symbol.hpp"



class PayTable {
private:
    string id, pattern_id;

    virtual bool isVaildMatch(const std::vector<Symbol>& result) const = 0;
public:
    PayTable() {};
    PayTable(string id, string pattern_id) : id(id), pattern_id(pattern_id) {};

    string getId() const;
    string getPatternId() const;

    SpinResult evaluate(const std::vector<Symbol>& result) const;
};

class PayTableMatching : public PayTable {
private:
    bool isVaildMatch(const std::vector<Symbol>& result) const;
public:
    PayTableMatching() {};
    PayTableMatching(string id, string pattern_id) : PayTable(id, pattern_id) {};
};

#endif  // PAY_TABLE_HPP
