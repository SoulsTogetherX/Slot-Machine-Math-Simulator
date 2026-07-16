#include "symbol/symbol.hpp"
#include "utils/types.hpp"



#pragma region Compairison
// Two symbols match if either is a wild (wilds substitute for anything) or they
// share the same id.
bool Symbol::matches(const Symbol& symbol) const {
    if (symbol.getType() == WILD || type == WILD) {
        return true;
    }
    return symbol.getId() == id;
}
// Two symbols match share the same id. Wilds are not considered.
bool Symbol::matchesExact(const Symbol& symbol) const {
    return symbol.getId() == id;
}
#pragma endregion

#pragma region Accessors
std::string Symbol::getId() const {
    return id;
}
SYMBOL_TYPE Symbol::getType() const {
    return type;
}
int Symbol::getMultiplier() const {
    return multiplier;
}
#pragma endregion