#ifndef SYMBOL_HPP
#define SYMBOL_HPP

#include "utilts/defs.hpp"



class Symbol {
private:
    string id;
    SYMBOL_TYPE type;
    int baseValue;
public:
    Symbol() {};
    Symbol(
        string id, SYMBOL_TYPE type, int value
    ) : id(id), type(type), baseValue(value) {};

    bool matches(Symbol symbol) const;

    string getId() const;
    SYMBOL_TYPE getType() const;
    int getBaseValue() const;
};

#endif  // SYMBOL_HPP
