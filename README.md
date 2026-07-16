# Slot Machine Math Simulator

A C++17 engine for designing and testing slot machine math models. Describe a
machine — symbols, weighted virtual reels, paylines, paytables — in a single
JSON file, run it through millions of simulated spins, and get accurate
statistics on it's use.

## Example Output

```
=====================  SESSION REPORT  --  100,000 spins  =====================

EXECUTIVE SUMMARY
-------------------------------------------------------------------------------
  Spins simulated              100,000    Hold (house edge)            -37.49%
  Wager / spin                   $1.00    Observed RTP                 137.49%
  Total wagered            $100,000.00    Hit frequency                100.00%
  Total returned           $137,488.00    Volatility                Low (0.95)
  Net result               +$37,488.00    RTP 95% CI            137.49% +/- 0.30%
  Longest win streak     100,000 spins    Longest loss streak          0 spins
-------------------------------------------------------------------------------

SYMBOL DISTRIBUTION (screen)
  CHERRY  ############............    50.0%  (300279)
  SEVEN   ######..................    25.0%  (150143)
  BAR     ######..................    24.9%  (149578)

PAYTABLE BREAKDOWN  (sorted by RTP contribution)
  Paytable  Pattern     Hit%  Consol%  RTP Contrib  Consol RTP  Avg Win
  --------  -------  -------  -------  -----------  ----------  -------
  pt1       p1       137.49%    0.00%      137.49%       0.00%     1.00
```

## Use Case

The main use of this project is to test the practicality of possible slot machines, without
relianing on theortical mathmatmatics. Using the Monte Carlo method, this project can help find
hidden patterns unnoticed.

With the current framework, it is trivial to add more (patterns), (value calculations), and (symbol types)
according to the slot machine you wish to test.

## How it works

This project relies on a `json` file to parse data from. The file `schema.json` (located in the folder
`data/`) contains all information of the current accepted properties. As such, I will only give a rough
overview of the most notable properties.

### Configuration format

A config file has two top-level sections:

```jsonc
{
  "slots_info": { ... },   // the machine itself (required)
  "test_info":  { ... }    // how to test it (optional)
}
```

**test_info** is meant to hold all information related to testing the slot machine, such as how many spins
the slot machine should be tested for, and the base bet every spin costs.

**slots_info** is meant to hold all information related to the slot machine itself, such as the reels and payouts.

For the rough overview, I will only focus on **slots_info**'s contained properties from this point.

### Symbols

This is a property where the symbols, which appear on the physical reel, are defined. Give them an id, a type,
and a multiplier.

The currently supported types are `normal` and `wild`. `wild` symbols can substitute for any other symbol.

To create custom types of symbols, you can create another symbol type.

### Reels

This property contains an array of reels, which can appear on the physical screen of the slot machine. Each reel is
an array of symbols, which you may attach a weight to in order to improve odds of landing on it.

### Patterns

This property contains objects used to define HOW the symbols are looked at for different payouts. The job of a "pattern"
is to extract certain symbols from the reels, after a spin, to be evaluated later.

The default pattern implemented is straight lines (Row, Column, Diagonal, Reverse Diagonal). If you wish to implement
your own patterns, you may extend from here.

### Payouts

Once patterns have extracted a subset of symbols, payouts define how those patterns are evaluated. If the user wins,
if the user loses, and how much money the user gains from a win or loss.

How this is determined depends on the payout. The default payout requires all symbols to match to count as a win.

### General Pipeflow

Through the `json` file, a user can define the symbols, how they are placed on the reels, the patterns the
slot machine looks for, and how certain patterns are payed out.

The flow follows: Machine is Spun > A screen of symbols is generated > the patterns extract symbols from the screen >
those symbols are then fed into patterns which output the slot machine's result.

## External Dependencies

For `json` parsing, the header file `[nlohmann/json](https://github.com/nlohmann/json)` is used, located
in the folder `third_party/`.

## Building

Requires a C++17 compiler and `make`.

**Linux / macOS:**

```sh
make
./app                 # runs data/examples/example.json
./app <filename>      # runs any file in data/examples/
```

**Windows (MSYS2 UCRT64/MinGW):**

```sh
make
./app.exe              # runs data/examples/example.json
./app.exe <filename>   # runs any file in data/examples/
```

`<filename>` is resolved relative to `data/examples/`. if no filename is given, it attempts to run
`example.json` automatically.

> If you hit `undefined reference to WinMain` while linking, something in
> your shell environment (usually a global `CXXFLAGS`/`LDFLAGS`) is forcing
> the GUI subsystem. The Makefile already passes `-mconsole` on Windows to
> guard against this, but if you still see it, run `echo $CXXFLAGS $LDFLAGS`
> to find the culprit.

`make clean` removes build artifacts. `make run` builds and runs in one step.

## Premade Example Files

| Example                  | Demonstrates                                                              |
| ------------------------ | ------------------------------------------------------------------------- |
| `example.json`           | Minimal machine. All `row` paytable                                       |
| `multi_line.json`        | Every `row` scored at once, with a wild                                   |
| `variants.json`          | Per-symbol variant payouts, including a wild-only jackpot and a catch-all |
| `virtual_stops.json`     | Weighted virtual reel stops                                               |
| `column_pattern.json`    | `col` patterns (a vertical line within one reel)                          |
| `diagonal_patterns.json` | Both diagonal directions on a 5-reel grid                                 |
| `multiplier_wild.json`   | Stacking symbol multipliers, including on a wild                          |
| `full_featured.json`     | Everything at once, run in progressive batches                            |

## License

Apache License 2.0 — see [`LICENSE`](LICENSE).

[nlohmann/json](https://github.com/nlohmann/json) is bundled by its own MIT license
(see `third_party/NOTICE.md`).
