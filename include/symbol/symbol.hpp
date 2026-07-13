#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include "utilts/defs.hpp"



// The Class that represents a 'Symbol' on a slotmachine, holding all needed information.
class Symbol {
private:
    string id; // The Symbol's name
    SYMBOL_TYPE type; // How the Symbol should act
    int baseValue; // The base value of the Symbol

public:
    Symbol() {};
    Symbol(
        string id, SYMBOL_TYPE type, int value
    ) : id(id), type(type), baseValue(value) {};

    // Accessor Methods
    string getId() const;
    SYMBOL_TYPE getType() const;
    int getBaseValue() const;

    // Checks if the given Symbol matches this Symbol.
    bool matches(Symbol symbol) const;
};

#endif  // SYMBOL_HPP
