#ifndef SYMBOL_TYPES_HPP
#define SYMBOL_TYPES_HPP

#include <vector>
#include <unordered_map>

#include "utils/types.hpp"
#include "symbol/symbol.hpp"

// A convolution: array of Symbols
using SymbolLine = std::vector<const Symbol*>;
// A full slot screen [reel][row]
using SymbolGrid = std::vector<std::vector<const Symbol*>>;
// Cache of convolutions produced by every referenced Pattern in a single spin.
// (pattern id, vector<SymbolLine>).
using ConvolutionCache = std::unordered_map<std::string, std::vector<SymbolLine>>;

#endif  // SYMBOL_TYPES_HPP
