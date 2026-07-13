#include "main/game.hpp"
#include "utilts/defs.hpp"

#include <fstream>
#include <iostream>



int main(int argc, char** argv) {
    std::string pathname = "data/example.json";
    if (argc > 1) {
        pathname = argv[1];
    }
    std::cout << "Reading JSON from: " << pathname << "\n\n";

    Game game = Game(pathname);

    return 0;
}
