#include <algorithm>

#include "virtual_reels/reel.hpp"
#include "utils/types.hpp"



#pragma region Assessor Methods
const Symbol* VirtualReel::getSymbolAt(uint idx) const {
    if (strip.empty()) {
        throw std::out_of_range("Index out of bounds: " + std::to_string(idx));
    }
    return strip[(current_pos + idx) % strip.size()];
}

// Appends a stop and grows the running weight total; 'dist' is rebuilt each call
// since its upper bound (total_weight - 1) changes as stops are added during setup.
void VirtualReel::addStop(const Symbol &s, uint weight) {
    strip.push_back(&s);
    total_weight += weight;
    cum_weights.push_back(total_weight);

    if (total_weight > 0) {
        dist = std::uniform_int_distribution<uint>(0, total_weight - 1);
    }
}

uint VirtualReel::getStripLength() const {
    return strip.size();
}
uint VirtualReel::getTotalWeight() const {
    return total_weight;
}
#pragma endregion

#pragma region Run Methods
// Picks a uniformly random virtual stop in [0, total_weight), then maps it to its
// physical stop via binary search on the cumulative weights (the first stop whose
// running total exceeds it). Heavier stops occupy a wider slice of the range and so
// land more often, independent of how many physical positions they occupy on the strip.
void VirtualReel::spin(std::mt19937 &rng) {
    if (total_weight == 0) {
        return;
    }

    const uint v = dist(rng);
    current_pos = static_cast<uint>(
        std::upper_bound(cum_weights.begin(), cum_weights.end(), v) - cum_weights.begin()
    );
}
#pragma endregion
