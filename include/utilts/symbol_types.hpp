#ifndef SYMBOL_TYPES_HPP
#define SYMBOL_TYPES_HPP

#include "symbol/symbol.hpp"

//      Complex types
// one convolution / payline
using SymbolLine = std::vector<const Symbol*>;
// a full slot screen [reel][row]
using SymbolGrid = std::vector<std::vector<const Symbol*>>;
// Cache of convolutions produced by every referenced Pattern in a single spin.
// (pattern id, vector<SymbolLine>).
using ConvolutionCache = std::unordered_map<string, std::vector<SymbolLine>>;

#endif  // SYMBOL_TYPES_HPP
