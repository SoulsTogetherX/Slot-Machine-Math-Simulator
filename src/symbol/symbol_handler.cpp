#include "symbol/symbol_handler.hpp"
#include "utilts/defs.hpp"
#include "utilts/extracts.hpp"



#pragma region Extract Info Helpers
void SymbolHandler::loadJson(const json& info) {
    extractSymbols(info.at("symbols"));
}

void SymbolHandler::extractSymbols(const json& info) {
    if (!info.is_array()) {
        throw std::invalid_argument("Symbols is expected to be array, but found: " + to_string(info));
    }

    for (auto it : info) {
        string id = extractStr("id", it);
        if (symbols.find(id) != symbols.end()) {
            throw std::invalid_argument("Symbol'" + id + "' was previously defined.");
        }
        
        SYMBOL_TYPE type;
        string str_type = extractStr("type", it);
        if (str_type == "normal") {
            type = NORMAL;
        } else if (str_type == "wild") {
            type = WILD;
        } else {
            throw std::invalid_argument("Unknown 'type' found.");
        }
        
        int value = extractInt("value", it, 1);
        symbols[id] = Symbol(id, type, value);
    }
}
#pragma endregion

#pragma region Helpers
bool SymbolHandler::hasSymbol(string id) const {
    return symbols.find(id) != symbols.end();
}
const Symbol& SymbolHandler::getSymbol(string id) const {
    return symbols.at(id);
}
std::vector<string> SymbolHandler::getSymbolIds() const {
    std::vector<string> ret;
    ret.reserve(symbols.size());

    for(auto sym : symbols) {
        ret.push_back(sym.first);
    }

    return ret;
}

void SymbolHandler::clear() {
    symbols.clear();
}
#pragma endregion