#ifndef TYPES_HPP
#define TYPES_HPP

// Lightweight, dependency-free shared types used throughout the project.

#include <string>
#include <stdexcept>

using uint = unsigned int;

// The functionality type of a symbol.
enum SYMBOL_TYPE {
    NORMAL,  // Normal
    WILD,    // Can substitute for any Symbol
    SCATTER, // Pays/triggers regardless of position; does not substitute
    BONUS    // Triggers a bonus feature; does not substitute
};

#endif  // TYPES_HPP
