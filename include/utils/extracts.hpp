#ifndef EXTRACTS_HPP
#define EXTRACTS_HPP

#include <optional>

#include <nlohmann/json.hpp>

#include "utils/types.hpp"



// A helper method to extract strings from nlohmann::json easier
inline std::string extractStr(const std::string& key, const nlohmann::json& j, std::optional<std::string> base = {}) {
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
// A helper method to extract ints from nlohmann::json easier
inline int extractInt(const std::string& key, const nlohmann::json& j, std::optional<int> base = {}) {
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
// A helper method to extract booleans from nlohmann::json easier
inline bool extractBool(const std::string& key, const nlohmann::json& j, std::optional<bool> base = {}) {
    if (!j.contains(key) || j.at(key).is_null()) {
        if (base) {
            return *base;
        }
        throw std::out_of_range("key '" + key + "' not found.");
    }

    auto info = j.at(key);
    if (!info.is_boolean()) {
        if (base) {
            return *base;
        }
        throw std::invalid_argument("Expected to find boolean, but found: " + to_string(info));
    }
    return info.get<bool>();
}
// A helper method to extract doubles from nlohmann::json easier
inline double extractDouble(const std::string& key, const nlohmann::json& j, std::optional<double> base = {}) {
    if (!j.contains(key) || j.at(key).is_null()) {
        if (base) {
            return *base;
        }
        throw std::out_of_range("key '" + key + "' not found.");
    }

    auto info = j.at(key);
    if (!info.is_number()) {
        if (base) {
            return *base;
        }
        throw std::invalid_argument("Expected to find number, but found: " + to_string(info));
    }
    return info.get<double>();
}

#endif  // EXTRACTS_HPP
