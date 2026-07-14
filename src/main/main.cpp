#include "main/slot_machine.hpp"
#include "utilts/defs.hpp"

#include <fstream>
#include <iostream>



int main(int argc, char** argv) {
    // Gets pathname
    std::string pathname = "data/examples/example.json";
    if (argc > 1) {
        pathname = argv[1];
    }
    std::cout << "Reading JSON from: " << pathname << "\n\n";

    // Creates the slotmachine
    SlotMachine machine = SlotMachine(pathname);

    for(int i = 0; i < 1000; i++) {
        machine.spinBet();
    }
    machine.printRaw();

    return 0;
}
