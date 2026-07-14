#ifndef DEFS_HPP
#define DEFS_HPP

#include <string>
#include <stdexcept>

#include "utilts/json.hpp"


//    ========== Shared types for better readability. ==========
//      Simplified types
using string = std::string;
using json = nlohmann::json;
using uint = unsigned int;

// The functionality type of a symbol
enum SYMBOL_TYPE {
    NORMAL, // Normal
    WILD    // Can be any Symbol
};

#endif  // DEFS_HPP
