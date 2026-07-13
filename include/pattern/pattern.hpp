#ifndef PATTERN_HPP
#define PATTERN_HPP

#include <string>

#include "utilts/defs.hpp"
#include "symbol/symbol.hpp"



class Pattern {
private:
    string id;

public:
    Pattern() {};
    Pattern(string id) : id(id) {};

    string getId() const;

    virtual std::vector<std::vector<Symbol>> convolution(
        const std::vector<std::vector<Symbol>> &results 
    ) const = 0;
};

class LinePattern : public Pattern {
private:
    uint row;

public:
    LinePattern();
    LinePattern(string id, uint row);

    std::vector<std::vector<Symbol>> convolution(
        const std::vector<std::vector<Symbol>> &results 
    ) const;
};

#endif  // PATTERN_HPP
