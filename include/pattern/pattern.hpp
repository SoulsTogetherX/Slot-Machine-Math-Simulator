#ifndef PATTERN_HPP
#define PATTERN_HPP

#include <string>
#include <unordered_map>

#include "utils/types.hpp"
#include "utils/symbol_types.hpp"
#include "stats/stats_handler.hpp"
#include "symbol/symbol.hpp"



// The abstract class used to produce convolutions of symbols from the slot screen.
// Overwrite 'handleConvolution' to handle different convolution types.
class Pattern {
private:
    std::string id; // The Pattern's name
public:
    Pattern() {};
    Pattern(std::string id) : id(id) {};

    // Accessor Methods
    std::string getId() const;

    // Returns one or more convolutions over the slot screen.
    virtual std::vector<SymbolLine> convolution(
        const SymbolGrid &results
    ) const = 0;

    // Returns a short human-readable description of this pattern's type and
    // configuration.
    // Default falls back to just the pattern's kind being unknown.
    virtual std::string describe() const { return "pattern"; }
};

#endif  // PATTERN_HPP
