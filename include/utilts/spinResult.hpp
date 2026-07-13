#ifndef SPIN_RESULT_HPP
#define SPIN_RESULT_HPP

#include <vector>

#include "utilts/defs.hpp"
#include "symbol/symbol.hpp"



struct SpinResult {
    std::vector<Symbol> symbols;
    
    string pattern_id;
    string payout_id;
    
    int payout;
    bool payoutWon;
};


#endif  // SPIN_RESULT_HPP
