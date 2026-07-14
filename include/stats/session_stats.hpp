#ifndef SESSION_STATS_HPP
#define SESSION_STATS_HPP

#include <cstdint>
#include <array>

#include "utilts/defs.hpp"



// Accumulates the financial outcome of a session of spins: money wagered, money
// won, money paid as consolation, and the derived slot-math figures (RTP, hit
// frequency, volatility). Won money and consolation money are tracked separately.
class SessionStats {
private:
    uint spins = 0;
    uint win_spins = 0;

    double total_wager = 0;
    double total_won = 0;           // money from winning (won == true) payouts
    double total_consolation = 0;   // money from non-winning (won == false) payouts

    double max_win = 0;             // largest single-spin total return

    // Running mean and sum-of-squared-deviations of the per-spin return ratio
    // (return / wager), maintained via Welford's algorithm for numerical stability
    // over very large sample runs.
    double mean_ratio = 0;
    double m2_ratio = 0;

    uint cur_loss_streak = 0;
    uint max_loss_streak = 0;
public:
    // Records one spin's outcome (won and consolation money for that spin).
    void record(double wager, double won, double consolation);

    uint getSpins() const;
    uint getWinSpins() const;
    double getTotalWager() const;
    double getTotalWon() const;
    double getTotalConsolation() const;
    double getTotalReturn() const;      // won + consolation
    double getMaxWin() const;
    uint getMaxLossStreak() const;

    // Derived math (all guard against division by zero)
    double rtp() const;                 // total return / total wager
    double rtpWon() const;              // won money / total wager
    double rtpConsolation() const;      // consolation money / total wager
    double hitFrequency() const;        // win spins / spins
    double avgWin() const;              // total won / win spins
    double variance() const;            // of the per-spin return ratio
    double stddev() const;
    double volatilityIndex(double z = 1.96) const;   // z * stddev
    double rtpMargin(double z = 1.96) const;         // CI half-width for RTP

    // Clears all session data
    void clear();
};

#endif  // SESSION_STATS_HPP
