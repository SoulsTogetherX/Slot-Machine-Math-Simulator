#ifndef PATTERN_HANDLER_HPP
#define PATTERN_HANDLER_HPP

#include <unordered_map>
#include <memory>
#include <nlohmann/json.hpp>

#include "utils/types.hpp"
#include "utils/symbol_types.hpp"
#include "pattern/pattern.hpp"
#include "virtual_reels/reel_handler.hpp"



// A Class used to store and evaluate Patterns, returning convolutions of symbols
// from the slot screen.
class PatternHandler {
private:
    // A map of all registered patterns (id, Pattern)
    // Stored in pointer form as Pattern is an abstract class
    std::unordered_map<std::string, std::unique_ptr<Pattern>> patterns;

    // Extracts Patterns from nlohmann::json.
    void extractPatterns(const nlohmann::json& info, const ReelHandler& reel_handler);
    std::vector<uint> extractRows(const nlohmann::json& it, uint payout_rows);
public:
    // Extracts Patterns from nlohmann::json.
    void loadJson(const nlohmann::json& info, const ReelHandler& reel_handler);

    // Convolves each of the given patterns over the screen exactly once, recording
    // each pattern's stats once, and returns the results keyed by pattern id.
    ConvolutionCache convolveAll(
        const SymbolGrid& screen, const std::vector<std::string>& pattern_ids
    );

    bool hasPattern(const std::string& id) const;
    Pattern& getPattern(const std::string& id) const;
    
    // Returns non-owning views of every stored Pattern.
    std::vector<const Pattern*> getAllPatterns() const;

    // Clears Patterns
    void clear();
};

#endif  // PATTERN_HANDLER_HPP
