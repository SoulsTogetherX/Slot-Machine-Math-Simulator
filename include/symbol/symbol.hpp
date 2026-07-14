#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include <vector>

#include "utilts/defs.hpp"



// The Class that represents a 'Symbol' on a slotmachine, holding all needed information.
class Symbol {
private:
    string id; // The Symbol's name
    SYMBOL_TYPE type; // How the Symbol should act
public:
    Symbol() {};
    Symbol(
        string id, SYMBOL_TYPE type
    ) : id(id), type(type) {};

    // Accessor Methods
    string getId() const;
    SYMBOL_TYPE getType() const;

    // Checks if the given Symbol matches this Symbol.
    bool matches(const Symbol& symbol) const;
};


#endif  // SYMBOL_HPP
