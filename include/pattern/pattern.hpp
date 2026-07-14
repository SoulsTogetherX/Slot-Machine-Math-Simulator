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
    StatsHandler stats;

    string id; // The Pattern's name

    // Returns one or more convolutions over the slot screen.
    virtual std::vector<SymbolLine> handleConvolution(
        const SymbolGrid &results
    ) const = 0;
public:
    Pattern() {};
    Pattern(string id) : id(id) {};

    // Accessor Methods
    string getId() const;
    const StatsHandler& getStats() const;

    // Calls 'handleConvolution' and handles storage of stats process.
    std::vector<SymbolLine> convolution(
        const SymbolGrid &results
    );
};

// The class that convolutions the slot screen in a horizontal row pattern.
class LinePattern : public Pattern {
private:
    uint row; // The row being convoluted over.

    std::vector<SymbolLine> handleConvolution(
        const SymbolGrid &results
    ) const override;
public:
    LinePattern() {};
    LinePattern(string id, uint row) : Pattern(id), row(row) {};
};

#endif  // PATTERN_HPP
