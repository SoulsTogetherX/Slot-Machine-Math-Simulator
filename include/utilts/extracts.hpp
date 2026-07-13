#ifndef EXTRACTS_HPP
#define EXTRACTS_HPP

#include <optional>

#include "utilts/defs.hpp"



// A helper method to extract strings from json easier
inline string extractStr(const string&key, const json& j, std::optional<string> base = {}) {
    if (!j.contains(key) || j.at(key).is_null()) {
        if (base) {
            return *base;
        }
        throw std::out_of_range("key '" + key + "' not found.");
    }

    auto info = j.at(key);
    if (!info.is_string()) {
        throw std::invalid_argument("Expected to find string, but found: " + to_string(info));
    }
    return info.get<std::string>();
}
// A helper method to extract ints from json easier
inline int extractInt(const string&key, const json& j, std::optional<int> base = {}) {
    if (!j.contains(key) || j.at(key).is_null()) {
        if (base) {
            return *base;
        }
        throw std::out_of_range("key '" + key + "' not found.");
    }

    auto info = j.at(key);
    if (!info.is_number_integer()) {
        if (base) {
            return *base;
        }
        throw std::invalid_argument("Expected to find integer, but found: " + to_string(info));
    }
    return info.get<int>();
}

#endif  // EXTRACTS_HPP
