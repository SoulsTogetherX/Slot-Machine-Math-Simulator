#ifndef PATTERN_HANDLER_HPP
#define PATTERN_HANDLER_HPP

#include <unordered_map>
#include <memory>

#include "utilts/defs.hpp"
#include "pattern/pattern.hpp"
#include "virtual_reels/reel_handler.hpp"



// A Class used to store and evaluate Patterns, returning convolutions of symbols
// from the slot screen.
class PatternHandler {
private:
    // A map of all registered patterns (id, Pattern)
    // Stored in pointer form as Pattern is an abstract class
    std::unordered_map<string, std::unique_ptr<Pattern>> patterns;

    // Extracts Patterns from json.
    void extractPatterns(const json& info, const ReelHandler& reel_handler);
public:
    // Extracts Patterns from json.
    void loadJson(const json& info, const ReelHandler& reel_handler);

    // Accessor Methods
    bool hasPattern(string id) const;
    Pattern& getPattern(string id) const;

    // Aggergate the accumulated stats from all patterns
    StatsHandler aggergateStats() const;

    // Clears Patterns
    void clear();
};

#endif  // PATTERN_HANDLER_HPP
