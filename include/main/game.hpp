#ifndef GAME_HPP
#define GAME_HPP

#include "utilts/defs.hpp"
#include "symbol/symbol_handler.hpp"
#include "virtual_reels/reel_handler.hpp"
#include "pattern/pattern_handler.hpp"
#include "paytable/paytable_handler.hpp"



class Game {
private:
    string name;

    SymbolHandler symbol_handler;
    ReelHandler reel_handler;
    PatternHandler pattern_handler;
    PayTableHandler paytable_handler;
public:
    Game();
    Game(string pathname);
    Game(const json& info);

    void loadPath(string pathname);
    void loadJson(const json& info);

    void spinBet();

    void clear();
};

#endif  // GAME_HPP
