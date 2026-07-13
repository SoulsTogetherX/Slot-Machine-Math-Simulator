#ifndef PATTERN_HANDLER_HPP
#define PATTERN_HANDLER_HPP

#include <unordered_map>
#include <memory>

#include "utilts/defs.hpp"
#include "pattern/pattern.hpp"
#include "virtual_reels/reel_handler.hpp"



class PatternHandler {
private:
    std::unordered_map<string, std::unique_ptr<Pattern>> patterns;

    void extractPatterns(const json& info, const ReelHandler& reel_handler);
public:
    void loadJson(const json& info, const ReelHandler& reel_handler);

    bool hasPattern(string id) const;
    const Pattern& getPattern(string id) const;

    void clear();
};

#endif  // PATTERN_HANDLER_HPP
