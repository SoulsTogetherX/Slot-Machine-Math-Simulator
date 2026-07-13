#include "virtual_reels/reel.hpp"
#include "utilts/defs.hpp"



#pragma region Symbol Methods
void VirtualReel::spin(std::mt19937 &rng) {
    currentPos = dist(rng);
}

Symbol VirtualReel::getSymbolAt(uint idx) const {
    if (reel.empty()) {
        throw std::out_of_range("Index out of bounds: " + std::to_string(idx));
    }
    return *reel.at((currentPos + idx) % reel.size());
}

void VirtualReel::addSymbol(const Symbol &s, uint repeat) {
    reel.insert(reel.end(), repeat, &s);
    dist = std::uniform_int_distribution<int>(0, static_cast<int>(reel.size()) - 1);
}
#pragma endregion

#pragma region Helpers
uint VirtualReel::getReelLength() const {
    return reel.size();
}
#pragma endregion