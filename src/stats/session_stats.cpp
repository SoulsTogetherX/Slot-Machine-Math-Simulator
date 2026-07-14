#include <cmath>

#include "stats/session_stats.hpp"



#pragma region Record
void SessionStats::record(double wager, double won, double consolation) {
    const double ret = won + consolation;

    spins += 1;
    total_wager += wager;
    total_won += won;
    total_consolation += consolation;

    if (won > 0) {
        win_spins += 1;
        cur_loss_streak = 0;
    } else {
        cur_loss_streak += 1;
        if (cur_loss_streak > max_loss_streak) {
            max_loss_streak = cur_loss_streak;
        }
    }

    if (ret > max_win) {
        max_win = ret;
    }

    // Welford update on the per-spin return ratio (return / wager).
    const double x = (wager > 0) ? ret / wager : 0.0;
    const double delta = x - mean_ratio;
    mean_ratio += delta / spins;
    m2_ratio += delta * (x - mean_ratio);
}
#pragma endregion

#pragma region Raw Accessors
uint SessionStats::getSpins() const {
    return spins;
}
uint SessionStats::getWinSpins() const {
    return win_spins;
}
double SessionStats::getTotalWager() const {
    return total_wager;
}
double SessionStats::getTotalWon() const {
    return total_won;
}
double SessionStats::getTotalConsolation() const {
    return total_consolation;
}
double SessionStats::getTotalReturn() const {
    return total_won + total_consolation;
}
double SessionStats::getMaxWin() const {
    return max_win;
}
uint SessionStats::getMaxLossStreak() const {
    return max_loss_streak;
}
#pragma endregion

#pragma region Derived Math
double SessionStats::rtp() const {
    return total_wager > 0 ? getTotalReturn() / total_wager : 0.0;
}
double SessionStats::rtpWon() const {
    return total_wager > 0 ? total_won / total_wager : 0.0;
}
double SessionStats::rtpConsolation() const {
    return total_wager > 0 ? total_consolation / total_wager : 0.0;
}
double SessionStats::hitFrequency() const {
    return spins > 0 ? static_cast<double>(win_spins) / static_cast<double>(spins) : 0.0;
}
double SessionStats::avgWin() const {
    return win_spins > 0 ? total_won / static_cast<double>(win_spins) : 0.0;
}
double SessionStats::variance() const {
    return spins > 0 ? m2_ratio / static_cast<double>(spins) : 0.0;
}
double SessionStats::stddev() const {
    return std::sqrt(variance());
}
double SessionStats::volatilityIndex(double z) const {
    return z * stddev();
}
double SessionStats::rtpMargin(double z) const {
    return spins > 0 ? volatilityIndex(z) / std::sqrt(static_cast<double>(spins)) : 0.0;
}
#pragma endregion

#pragma region Helpers
void SessionStats::clear() {
    spins = 0;
    win_spins = 0;
    total_wager = 0;
    total_won = 0;
    total_consolation = 0;
    max_win = 0;
    mean_ratio = 0;
    m2_ratio = 0;
    cur_loss_streak = 0;
    max_loss_streak = 0;
}
#pragma endregion
