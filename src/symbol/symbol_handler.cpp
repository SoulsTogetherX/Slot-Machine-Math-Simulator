#include <nlohmann/json.hpp>

#include "symbol/symbol_handler.hpp"
#include "utils/types.hpp"
#include "utils/extracts.hpp"



#pragma region Extract Info Helpers
void SymbolHandler::loadJson(const nlohmann::json& info) {
    extractSymbols(info.at("symbols"));
}

void SymbolHandler::extractSymbols(const nlohmann::json& info) {
    if (!info.is_array()) {
        throw std::invalid_argument("Symbols is expected to be array, but found: " + to_string(info));
    }

    for (const auto& it : info) {
        const std::string id = extractStr("id", it);
        if (symbols.find(id) != symbols.end()) {
            throw std::invalid_argument("Symbol'" + id + "' was previously defined.");
        }
        
        const std::string str_type = extractStr("type", it, "normal");
        SYMBOL_TYPE type;

        if (str_type == "normal") {
            type = NORMAL;
        } else if (str_type == "wild") {
            type = WILD;
        } else {
            throw std::invalid_argument("Unknown 'type' found: " + str_type);
        }

        const int multiplier = extractInt("multiplier", it, 1);
        symbols[id] = Symbol(id, type, multiplier);
    }
}
#pragma endregion

#pragma region Accessor Methods
bool SymbolHandler::hasSymbol(const std::string& id) const {
    return symbols.find(id) != symbols.end();
}
const Symbol& SymbolHandler::getSymbol(const std::string& id) const {
    return symbols.at(id);
}

std::vector<std::string> SymbolHandler::getSymbolIds() const {
    std::vector<std::string> ret;
    ret.reserve(symbols.size());

    for(const auto& sym : symbols) {
        ret.push_back(sym.first);
    }

    return ret;
}
#pragma endregion

#pragma region Helpers
void SymbolHandler::clear() {
    symbols.clear();
}
#pragma endregion