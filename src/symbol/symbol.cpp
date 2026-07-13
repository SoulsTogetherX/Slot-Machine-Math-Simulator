#include "symbol/symbol.hpp"
#include "utilts/defs.hpp"



#pragma region Compairison
bool Symbol::matches(const Symbol& symbol) const {
    if (symbol.getType() == WILD || type == WILD) {
        return true;
    }
    return symbol.getId() == id;
}
#pragma endregion

#pragma region Accessors
string Symbol::getId() const {
    return id;
}
SYMBOL_TYPE Symbol::getType() const {
    return type;
}
#pragma endregion