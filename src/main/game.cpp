#include <fstream>

#include "utilts/defs.hpp"
#include "main/game.hpp"
#include "utilts/extracts.hpp"
#include "virtual_reels/reel.hpp"
#include "pattern/pattern.hpp"



#pragma region Constructors
Game::Game() {
    clear();
}
Game::Game(string pathname) {
    loadPath(pathname);
}
Game::Game(const json& data) {
    loadJson(data);
}
#pragma endregion endregion


#pragma region Load Info
void Game::loadPath(string pathname) {
    clear();

    std::ifstream file(pathname);
    if (!file) {
        throw std::runtime_error("Could not open file: " + pathname);
    }

    json data;
    file >> data;
    loadJson(data);
}
#include <iostream>
void Game::loadJson(const json& data) {
    clear();
    
    // Id
    name = extractStr("name", data);

    // Symbols
    symbol_handler.loadJson(data);

    // Virtual Reels
    reel_handler.loadJson(data, symbol_handler);

    // Patterns
    pattern_handler.loadJson(data, reel_handler);

    // Paytable
    paytable_handler.loadJson(data, pattern_handler);

    spinBet();
}
#pragma endregion


#pragma region Helpers
void Game::spinBet() {
    auto results = reel_handler.runSpin();
    paytable_handler.evaluateAll(results, pattern_handler);
}

void Game::clear() {
    name = "";

    symbol_handler.clear();
    reel_handler.clear();
    pattern_handler.clear();
    paytable_handler.clear();
}
#pragma endregion