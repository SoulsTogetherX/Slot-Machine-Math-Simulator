#ifndef LINE_PATTERN_HPP
#define LINE_PATTERN_HPP

#include "pattern/pattern.hpp"



// The abstract method for patterns features multiple straight line convolutions over the screen.
class LinePattern : public Pattern {
protected:
    std::vector<uint> lines; // The lines being convoluted over.

    template <typename F>
    std::vector<SymbolLine> convolution_helper(
        F lambda, std::vector<SymbolLine> &ret, uint max
    ) const;

    // The short name of this line pattern's orientation (e.g. "row", "col").
    virtual std::string typeName() const = 0;
public:
    LinePattern() : Pattern() {};
    LinePattern(std::string id, std::vector<uint> lines) : Pattern(id), lines(std::move(lines)) {};

    // Describes the pattern as its orientation plus the explicit 1-based line
    // indexes configured, or "(all)" when 'index' was omitted (every line used).
    std::string describe() const override;
};



// The class that convolutions the slot screen into horizontal row paylines.
class RowPattern : public LinePattern {
protected:
    std::string typeName() const override { return "row"; }
public:
    RowPattern() {};
    RowPattern(std::string id, std::vector<uint> rows) : LinePattern(id, rows) {};

    std::vector<SymbolLine> convolution(
        const SymbolGrid &results
    ) const override;
};

// The class that convolutions the slot screen into vertical column paylines.
class ColumnPattern : public LinePattern {
protected:
    std::string typeName() const override { return "col"; }
public:
    ColumnPattern() {};
    ColumnPattern(std::string id, std::vector<uint> columns) : LinePattern(id, columns) {};

    std::vector<SymbolLine> convolution(
        const SymbolGrid &results
    ) const override;
};

// The class that convolutions the slot screen into diagonal paylines (down-and-right).
class DiagonalPattern : public LinePattern {
protected:
    std::string typeName() const override { return "diagonal"; }
public:
    DiagonalPattern() {};
    DiagonalPattern(std::string id, std::vector<uint> columns) : LinePattern(id, columns) {};

    std::vector<SymbolLine> convolution(
        const SymbolGrid &results
    ) const override;
};

// The class that convolutions the slot screen into vertical column paylines (down-and-left).
class DiagonalReversedPattern : public LinePattern {
protected:
    std::string typeName() const override { return "diagonalReversed"; }
public:
    DiagonalReversedPattern() {};
    DiagonalReversedPattern(std::string id, std::vector<uint> columns) : LinePattern(id, columns) {};

    std::vector<SymbolLine> convolution(
        const SymbolGrid &results
    ) const override;
};

#endif  // LINE_PATTERN_HPP
