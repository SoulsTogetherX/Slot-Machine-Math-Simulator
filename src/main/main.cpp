#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>


#include "main/slot_machine_tester.hpp"
#include "utils/types.hpp"



#pragma region Constants
const std::string PATH_BASE = "data/examples/";
#pragma endregion

#pragma region Mains
int main(int argc, char** argv) {
    // Gets pathname
    std::string pathname;
    if (argc > 1) {
        pathname = PATH_BASE + argv[1];
    } else {
        pathname = PATH_BASE + "example.json";
    }
    std::cout << "Reading JSON from: " << pathname << "\n\n";

    // Gets Json
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
#pragma endregion
