#include <iostream>
#include <algorithm>
#include <utility>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>

#include "main/slot_machine_tester.hpp"
#include "paytable/paytable.hpp"
#include "utils/extracts.hpp"



#pragma region Constructors
SlotMachineTester::SlotMachineTester() {
    bet_wager = 1;
    slot_machine = SlotMachine();
}
SlotMachineTester::SlotMachineTester(SlotMachine slots, uint bet_wager) {
    slot_machine = std::move(slots);
    this->bet_wager = bet_wager;
}
SlotMachineTester::SlotMachineTester(const nlohmann::json& info) {
    slot_machine = SlotMachine();
    loadJson(info);
}
#pragma endregion

#pragma region Load Info
void SlotMachineTester::loadJson(const nlohmann::json& info) {
    slot_machine.loadJson(info.at("slots_info"));

    printConfig();

    if (hasKey("test_info", info)) {
        const auto test_info = info.at("test_info");
        // Get bet amount
        bet_wager = extractInt("bet", test_info, 1);

        // Returns if no test spins requested
        if (!hasKey("test_spins", test_info)) {
            return;
        }

        // Spins requested test spins
        const auto test_spins = test_info.at("test_spins");
        if (test_spins.is_number_integer()) {
            runTestRun(test_spins.get<uint>());
        } else if (test_spins.is_array()) {
            auto rounds = std::vector<uint>();
            for(const auto& it : test_spins) {
                rounds.push_back(it);
            }
            runTestRun(rounds);
        } else {
            throw std::invalid_argument("Expected to find number or array, but found: " + to_string(info));
        }
    } else {
        // Default bet amount
        bet_wager = 1;
    }

}
#pragma endregion

#pragma region Test Runs
void SlotMachineTester::runTestRun(uint rounds) {
    for(uint i = 0; i < rounds; i++) {
        const auto betResult = slot_machine.spinBet();
        session.record(bet_wager, betResult.winPayout, betResult.losePayout);
    }

    printResults();
}
void SlotMachineTester::runTestRun(const std::vector<uint>& roundsMass) {
    uint i = 0;
    for(uint rounds : roundsMass) {
        for(; i < rounds; i++) {
            const auto betResult = slot_machine.spinBet();
            session.record(bet_wager, betResult.winPayout, betResult.losePayout);
        }

        printResults();
    }
}
#pragma endregion

#pragma region Formatting Helpers (local)
namespace {
    std::string symbolTypeName(SYMBOL_TYPE type) {
        switch (type) {
            case NORMAL:  return "normal";
            case WILD:    return "wild";
            case SCATTER: return "scatter";
            case BONUS:   return "bonus";
            default:      return "unknown";
        }
    }

    // Renders an integer string with thousands separators (e.g. 1234567 -> "1,234,567").
    std::string commas_str(const std::string& digits) {
        std::string out;
        int sinceGroup = 0;

        for (uint i = static_cast<uint>(digits.size()) - 1;; i--) {
            out.push_back(digits[static_cast<size_t>(i)]);
            if (i == 0) {
                break;
            }
            
            sinceGroup++;
            if (sinceGroup == 3) {
                out.push_back(',');
                sinceGroup = 0;
            }
        }
        std::reverse(out.begin(), out.end());
        return out;
    }
    // Renders an integer with thousands separators (e.g. 1234567 -> "1,234,567").
    std::string commas(long long value) {
        const bool neg = value < 0;
        unsigned long long v = neg ? static_cast<unsigned long long>(-value) : static_cast<unsigned long long>(value);
        return (neg ? "-" : "") + commas_str(std::to_string(v));
    }

    // Renders a credit amount with thousands separators and 2 decimals (e.g. "$1,234.56").
    // Built entirely from integer arithmetic (never formats a double and re-parses
    // it for a literal '.') so it can't be thrown off by a locale whose decimal
    // separator isn't '.', or by find()/substr() edge cases around that assumption.
    std::string money(double value) {
        const bool neg = value < 0;
        const double v = neg ? -value : value;

        // Guard against non-finite input (shouldn't occur given how this is used,
        // but a plain, harmless fallback beats crashing on the rounding below).
        if (!std::isfinite(v)) {
            return neg ? "-$?" : "$?";
        }

        const long long cents = std::llround(v * 100.0);
        const long long wholePart = cents / 100;
        const long long fracPart = cents % 100;

        std::ostringstream out;
        out << (neg ? "-$" : "$")
            << commas_str(std::to_string(wholePart))
            << "." << std::setfill('0')
            << std::setw(2)
            << fracPart;
        return out.str();
    }

    // A percentage formatted to 2 decimals with a trailing '%'.
    std::string pct(double value) {
        std::ostringstream out;
        out << std::fixed << std::setprecision(2) << value << "%";
        return out.str();
    }

    // A fixed-point number to 2 decimals (no unit suffix).
    std::string num(double value) {
        std::ostringstream out;
        out << std::fixed << std::setprecision(2) << value;
        return out.str();
    }

    // A meter bar made of '#' (filled) and '.' (empty) representing 'fraction' (0..1)
    // of 'width' characters. Plain ASCII on purpose, for maximum terminal/codepage
    // compatibility (this runs in the Windows console as often as a Unix terminal).
    std::string meter(double fraction, int width) {
        if (fraction < 0) fraction = 0;
        if (fraction > 1) fraction = 1;
        const int filled = static_cast<int>(std::lround(fraction * width));
        return std::string(static_cast<size_t>(filled), '#')
             + std::string(static_cast<size_t>(width - filled), '.');
    }

    // A rough, clearly-labeled-as-heuristic classification of session volatility,
    // the kind of shorthand a casino floor team would actually reach for.
    std::string volatilityLabel(double index) {
        if (index < 1.0) return "Low";
        if (index < 3.0) return "Medium";
        if (index < 6.0) return "High";
        return "Extreme";
    }

    // Prints a left-padded rule of '=' or '-' the given width.
    void rule(char c, int width = 79) {
        std::cout << std::string(static_cast<size_t>(width), c) << "\n";
    }

    // Prints a centered heading inside a rule, e.g. "===  TITLE  ===".
    void heading(const std::string& title, int width = 79) {
        const std::string label = "  " + title + "  ";
        int pad = width - static_cast<int>(label.size());
        if (pad < 0) pad = 0;
        const int left = pad / 2;
        const int right = pad - left;
        std::cout << std::string(static_cast<size_t>(left), '=') << label
                   << std::string(static_cast<size_t>(right), '=') << "\n";
    }

    // A minimal, dependency-free table renderer: auto-sized columns, a header row,
    // a divider, and left/right alignment per column (numbers right-aligned, text left).
    void printTable(
        const std::vector<std::string>& headers,
        const std::vector<std::vector<std::string>>& rows,
        const std::vector<bool>& rightAlign,
        const std::string& indent = "  "
    ) {
        const size_t cols = headers.size();
        std::vector<size_t> widths(cols);
        for (size_t c = 0; c < cols; c++) {
            widths[c] = headers[c].size();
        }
        for (const auto& row : rows) {
            for (size_t c = 0; c < cols; c++) {
                widths[c] = std::max(widths[c], row[c].size());
            }
        }

        auto printRow = [&](const std::vector<std::string>& row) {
            std::cout << indent;
            for (size_t c = 0; c < cols; c++) {
                std::cout << (rightAlign[c] ? std::right : std::left)
                           << std::setw(static_cast<int>(widths[c])) << row[c];
                if (c + 1 < cols) {
                    std::cout << "  ";
                }
            }
            std::cout << "\n";
        };

        printRow(headers);
        std::cout << indent;
        for (size_t c = 0; c < cols; c++) {
            std::cout << std::string(widths[c], '-');
            if (c + 1 < cols) {
                std::cout << "  ";
            }
        }
        std::cout << "\n";
        for (const auto& row : rows) {
            printRow(row);
        }
    }

    // Renders a StatsHandler's symbol distribution as a sorted (highest-share first)
    // bar-meter list instead of a wall of "SYMBOL: n (pct%)" lines.
    void printSymbolBars(const StatsHandler& stats, const std::string& indent, int barWidth = 20) {
        const auto counts = stats.getSymbolCounts();
        uint total = 0;
        for (const auto& [key, value] : counts) {
            total += value;
        }

        std::vector<std::pair<std::string, uint>> items(counts.begin(), counts.end());
        std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) {
            if (a.second != b.second) return a.second > b.second;
            return a.first < b.first;
        });

        size_t labelWidth = 0;
        for (const auto& [key, value] : items) {
            labelWidth = std::max(labelWidth, key.size());
        }

        for (const auto& [key, value] : items) {
            const double fraction = total > 0 ? static_cast<double>(value) / static_cast<double>(total) : 0.0;
            std::cout << indent
                       << std::left << std::setw(static_cast<int>(labelWidth)) << key << "  "
                       << meter(fraction, barWidth) << "  "
                       << std::right << std::setw(6) << std::fixed << std::setprecision(1) << (fraction * 100.0) << "%"
                       << "  (" << value << ")\n";
        }
        if (items.empty()) {
            std::cout << indent << "(no data)\n";
        }
    }
}
#pragma endregion

#pragma region Config
void SlotMachineTester::printConfig() const {
    const auto& symbol_handler = slot_machine.getSymbolHandler();
    const auto& reel_handler = slot_machine.getReelHandler();
    const auto& pattern_handler = slot_machine.getPatternHandler();
    const auto& paytable_handler = slot_machine.getPayTableHandler();

    heading("MACHINE CONFIGURATION");

    // Symbols
    auto symbol_ids = symbol_handler.getSymbolIds();
    std::sort(symbol_ids.begin(), symbol_ids.end());

    std::vector<std::vector<std::string>> symbol_rows;
    for (const auto& id : symbol_ids) {
        const Symbol& sym = symbol_handler.getSymbol(id);
        symbol_rows.push_back({
            id,
            symbolTypeName(sym.getType()),
            sym.getMultiplier() != 1 ? ("x" + std::to_string(sym.getMultiplier())) : "-"
        });
    }
    std::cout << "\nSymbols (" << symbol_ids.size() << "):\n";
    printTable({"Id", "Type", "Multiplier"}, symbol_rows, {false, false, true});

    // Reels
    std::cout << "\nReels: " << reel_handler.getReelCount()
              << "  |  Payout rows (visible window): " << reel_handler.getPayoutRows() << "\n";
    for (uint i = 0; i < reel_handler.getReelCount(); i++) {
        std::cout << "  Reel " << (i + 1) << ": " << reel_handler.getReelLength(i) << " physical stop(s)\n";
    }

    // Patterns
    auto patterns = pattern_handler.getAllPatterns();
    std::sort(patterns.begin(), patterns.end(), [](const Pattern* a, const Pattern* b) {
        return a->getId() < b->getId();
    });
    std::vector<std::vector<std::string>> pattern_rows;
    for (const Pattern* pattern : patterns) {
        pattern_rows.push_back({pattern->getId(), pattern->describe()});
    }
    std::cout << "\nPatterns (" << patterns.size() << "):\n";
    printTable({"Id", "Lines"}, pattern_rows, {false, false});

    // Paytables
    auto paytables = paytable_handler.getAllPayTables();
    std::sort(paytables.begin(), paytables.end(), [](const PayTable* a, const PayTable* b) {
        return a->getId() < b->getId();
    });
    std::vector<std::vector<std::string>> paytable_rows;
    for (const PayTable* paytable : paytables) {
        const auto* matching = dynamic_cast<const PayTableMatching*>(paytable);
        std::string variantInfo = "-";
        if (matching != nullptr && matching->hasVariants()) {
            variantInfo = std::to_string(matching->getVariants().size()) + " variant(s)";
        }
        paytable_rows.push_back({paytable->getId(), paytable->getPatternId(), variantInfo});
    }
    std::cout << "\nPaytables (" << paytables.size() << "):\n";
    printTable({"Id", "Pattern", "Scoring"}, paytable_rows, {false, false, false});

    rule('=');
    std::cout << "\n";
}
#pragma endregion

#pragma region Results
void SlotMachineTester::printResults() const {
    const auto& reel_handler = slot_machine.getReelHandler();
    const auto& paytable_handler = slot_machine.getPayTableHandler();

    const StatsHandler& reel_stats = reel_handler.getStats();
    const uint spins = reel_stats.getCount();
    const double total_wagered = session.getTotalWager();
    const double net_result = session.getTotalReturn() - session.getTotalWager();
    const double hold_pct = 100.0 - (100.0 * session.rtp());

    heading("SESSION REPORT  --  " + commas(spins) + " spins");

    // -------------------- Executive summary --------------------
    std::cout << "\nEXECUTIVE SUMMARY\n";
    rule('-', 79);
    {
        std::ostringstream left1, right1, left2, right2, left3, right3, left4, right4;
        left1  << "  Spins simulated       " << std::setw(14) << std::right << commas(spins);
        right1 << "  Hold (house edge)     " << std::setw(14) << std::right << pct(hold_pct);
        left2  << "  Wager / spin          " << std::setw(14) << std::right << money(bet_wager);
        right2 << "  Observed RTP          " << std::setw(14) << std::right << pct(100.0 * session.rtp());
        left3  << "  Total wagered         " << std::setw(14) << std::right << money(total_wagered);
        right3 << "  Hit frequency         " << std::setw(14) << std::right << pct(100.0 * session.hitFrequency());
        left4  << "  Total returned        " << std::setw(14) << std::right << money(session.getTotalReturn());
        right4 << "  Volatility            " << std::setw(14) << std::right
               << (volatilityLabel(session.volatilityIndex()) + " (" + num(session.volatilityIndex()) + ")");

        std::cout << std::left << std::setw(40) << left1.str()  << right1.str()  << "\n";
        std::cout << std::left << std::setw(40) << left2.str()  << right2.str()  << "\n";
        std::cout << std::left << std::setw(40) << left3.str()  << right3.str()  << "\n";
        std::cout << std::left << std::setw(40) << left4.str()  << right4.str()  << "\n";

        std::ostringstream left5, right5, left6, right6;
        left5  << "  Net result            " << std::setw(14) << std::right
               << (std::string(net_result >= 0 ? "+" : "") + money(net_result));
        right5 << "  RTP 95% CI            " << std::setw(14) << std::right
               << (pct(100.0 * session.rtp()) + " +/- " + pct(100.0 * session.rtpMargin()));
        left6  << "  Longest win streak    " << std::setw(14) << std::right << (commas(session.getMaxWinStreak()) + " spins");
        right6 << "  Longest loss streak   " << std::setw(14) << std::right << (commas(session.getMaxLossStreak()) + " spins");

        std::cout << std::left << std::setw(40) << left5.str() << right5.str() << "\n";
        std::cout << std::left << std::setw(40) << left6.str() << right6.str() << "\n";
    }
    rule('-', 79);

    // -------------------- Screen symbol distribution --------------------
    std::cout << "\nSYMBOL DISTRIBUTION (screen)\n";
    printSymbolBars(reel_stats, "  ", 24);

    // -------------------- Paytable breakdown --------------------
    auto paytables = paytable_handler.getAllPayTables();
    std::vector<const PayTable*> sorted_paytables(paytables.begin(), paytables.end());
    std::sort(sorted_paytables.begin(), sorted_paytables.end(), [](const PayTable* a, const PayTable* b) {
        return a->getStats().getTotalPayout() > b->getStats().getTotalPayout();
    });

    std::cout << "\nPAYTABLE BREAKDOWN  (sorted by RTP contribution)\n";
    std::vector<std::vector<std::string>> pt_rows;
    for (const PayTable* paytable : sorted_paytables) {
        const StatsHandler& pt_stats = paytable->getStats();
        const uint hits = pt_stats.getCount();
        const uint consolations = pt_stats.getConsolationCount();
        const double pt_payout = pt_stats.getTotalPayout();
        const double pt_consolation_payout = pt_stats.getTotalConsolationPayout();

        pt_rows.push_back({
            paytable->getId(),
            paytable->getPatternId(),
            spins > 0 ? pct(100.0 * hits / spins) : "-",
            spins > 0 ? pct(100.0 * consolations / spins) : "-",
            total_wagered > 0 ? pct(100.0 * pt_payout / total_wagered) : "-",
            total_wagered > 0 ? pct(100.0 * pt_consolation_payout / total_wagered) : "-",
            hits > 0 ? num(pt_payout / hits) : "-"
        });
    }
    printTable(
        {"Paytable", "Pattern", "Hit%", "Consol%", "RTP Contrib", "Consol RTP", "Avg Win"},
        pt_rows, {false, false, true, true, true, true, true}
    );

    // -------------------- Per-paytable / per-variant detail --------------------
    std::cout << "\nPAYTABLE DETAIL\n";
    for (const PayTable* paytable : sorted_paytables) {
        const StatsHandler& pt_stats = paytable->getStats();
        std::cout << "\n  == " << paytable->getId() << "  (pattern '" << paytable->getPatternId() << "') ==\n";
        std::cout << "  Winning symbol distribution:\n";
        printSymbolBars(pt_stats, "    ", 20);

        const auto* matching = dynamic_cast<const PayTableMatching*>(paytable);
        if (matching != nullptr && matching->hasVariants()) {
            const double pt_payout = pt_stats.getTotalPayout();
            const auto& all_variants = matching->getVariants();

            // Pair each variant with its 1-based position in the config so the
            // display index stays stable/meaningful even after sorting by payout.
            std::vector<std::pair<size_t, const PayoutVariant*>> indexed_variants;
            for (size_t i = 0; i < all_variants.size(); i++) {
                indexed_variants.push_back({i + 1, &all_variants[i]});
            }
            std::sort(indexed_variants.begin(), indexed_variants.end(), [](const auto& a, const auto& b) {
                return a.second->stats.getTotalPayout() > b.second->stats.getTotalPayout();
            });

            std::cout << "\n  Variant breakdown:\n";
            std::vector<std::vector<std::string>> variant_rows;
            for (const auto& [index, variant] : indexed_variants) {
                const StatsHandler& v_stats = variant->stats;
                const uint v_hits = v_stats.getCount();
                const uint v_consolations = v_stats.getConsolationCount();
                const double v_payout = v_stats.getTotalPayout();

                variant_rows.push_back({
                    "#" + std::to_string(index),
                    spins > 0 ? pct(100.0 * v_hits / spins) : "-",
                    spins > 0 ? pct(100.0 * v_consolations / spins) : "-",
                    v_hits > 0 ? num(v_payout / v_hits) : "-",
                    pt_payout > 0 ? pct(100.0 * v_payout / pt_payout) : "-",
                    variant->withWilds ? "yes" : "no"
                });
            }
            printTable(
                {"Variant", "Hit%", "Consol%", "Avg Win", "Share of PT RTP", "Wilds"},
                variant_rows, {false, true, true, true, true, true}, "    "
            );
        }
    }

    rule('=');
    std::cout << "\n\n\n";
}
#pragma endregion

#pragma region Helper
void SlotMachineTester::clear() {
    bet_wager = 1;
    slot_machine.clear();
    session.clear();
}
#pragma endregion