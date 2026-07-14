#ifndef PATTERN_HPP
#define PATTERN_HPP

#include <string>
#include <unordered_map>

#include "utilts/defs.hpp"
#include "utilts/symbol_types.hpp"
#include "stats/stats_handler.hpp"
#include "symbol/symbol.hpp"



// The abstract class used to produce convolutions of symbols from the slot screen.
// Overwrite 'handleConvolution' to handle different convolution types.
class Pattern {
private:
    string id; // The Pattern's name
public:
    Pattern() {};
    Pattern(string id) : id(id) {};

    // Accessor Methods
    string getId() const;

    // Returns one or more convolutions over the slot screen.
    virtual std::vector<SymbolLine> convolution(
        const SymbolGrid &results
    ) const = 0;
};

// The class that convolutions the slot screen into horizontal row paylines.
class LinePattern : public Pattern {
private:
    std::vector<uint> rows; // The rows being convoluted over.
public:
    LinePattern() {};
    LinePattern(string id, std::vector<uint> rows) : Pattern(id), rows(std::move(rows)) {};

    std::vector<SymbolLine> convolution(
        const SymbolGrid &results
    ) const override;
};

#endif  // PATTERN_HPP
