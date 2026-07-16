#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <vector>

#include "utils/types.hpp"



// The Class that represents a 'Symbol' on a slotmachine, holding all needed information.
class Symbol {
private:
    std::string id; // The Symbol's name
    SYMBOL_TYPE type; // How the Symbol should act
    int multiplier = 1; // Multiplies the payout of a win it takes part in
public:
    Symbol() {};
    Symbol(
        std::string id, SYMBOL_TYPE type, int multiplier = 1
    ) : id(id), type(type), multiplier(multiplier) {};

    std::string getId() const;
    SYMBOL_TYPE getType() const;
    int getMultiplier() const;

    // Checks if the given Symbol matches this Symbol (including wilds).
    bool matches(const Symbol& symbol) const;
    // Checks if the given Symbol matches this Symbol.
    bool matchesExact(const Symbol& symbol) const;
};


#endif  // SYMBOL_HPP
