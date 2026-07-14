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
    NORMAL,  // Normal
    WILD,    // Can substitute for any Symbol
    SCATTER, // Pays/triggers regardless of position; does not substitute
    BONUS    // Triggers a bonus feature; does not substitute
};

#endif  // DEFS_HPP
