#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>


#include "main/slot_machine_tester.hpp"
#include "utils/types.hpp"



// Entry point: loads a machine config (path from argv[1], falling back to the
// bundled example), then runs a fixed batch of test spins and prints the results.
int main(int argc, char** argv) {
    // Gets pathname
    std::string pathname = "data/examples/example.json";
    if (argc > 1) {
        pathname = argv[1];
    }
    std::cout << "Reading JSON from: " << pathname << "\n\n";

    std::ifstream file(pathname);
    if (!file) {
        throw std::runtime_error("Could not open file: " + pathname);
    }
    nlohmann::json data;
    file >> data;

    // Creates and runs the Slotmachine Tester
    auto tester = SlotMachineTester(data);

    return 0;
}
