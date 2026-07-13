#ifndef DEFS_HPP
#define DEFS_HPP

#include <string>
#include <stdexcept>

#include "utilts/json.hpp"

using string = std::string;
using json = nlohmann::json;
using uint = unsigned int;

enum SYMBOL_TYPE {
    NORMAL,
    WILD
};

#endif  // DEFS_HPP
