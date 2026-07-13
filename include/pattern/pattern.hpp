#ifndef PATTERN_HPP
#define PATTERN_HPP

#include <string>

#include "utilts/defs.hpp"
#include "stats/stats_handler.hpp"
#include "symbol/symbol.hpp"



class Pattern {
private:
    string id;
    StatsHandler stats;

    virtual std::vector<std::vector<Symbol>> handleConvolution(
        const std::vector<std::vector<Symbol>> &results 
    ) const = 0;
public:
    Pattern() {};
    Pattern(string id) : id(id) {};

    string getId() const;
    StatsHandler getStats() const;

    std::vector<std::vector<Symbol>> convolution(
        const std::vector<std::vector<Symbol>> &results 
    );
};

class LinePattern : public Pattern {
private:
    uint row;

    std::vector<std::vector<Symbol>> handleConvolution(
        const std::vector<std::vector<Symbol>> &results 
    ) const;
public:
    LinePattern();
    LinePattern(string id, uint row);
};

#endif  // PATTERN_HPP
