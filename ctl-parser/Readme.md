## CTL-Parser Library

The CTL-Parser library can parse CTL* formulas and returns an abstract syntax
tree of the parsed formula.

The CTL* formulas that are supported by the parser implement the following
grammar:

 * boolean literal (`true`, `false`) is a statement.
 * atoms (`[a-z]+`) are statements. Only lower-case atom names are supported.
 * negations (`not <statement>`) are statements.
 * conjunctions (`<statement> and <statement>`) are statements.
 * Exists next (`EX(<statement>)`) is a statement.
 * Exists globally (`EG(<statement>)`) is a statement.
 * Exists future (`EF(<statement>`) is a statement.
 * Exists until (`E <statement> U <statement>``) is a statement.
 * Always next (`AX (<statement>)`) is a statement.
 * Always globally (`AG (<statement>)`) is a statement.
 * Always future (`AF (<statement>)`) is a statement.
 * Always until (`A <statement> U <statement`) is a statement.

The abstract syntax tree that is returned by the parser represents the formula
as a minimal set of CTL operators consisting of the following
statements:

 * boolean literals, atoms, negations, conjunctions, exists next,
   exists globally, exists until.

The CTL-Parser is implemented as a static library and must be linked
into the target program.

The parser can be used via the following API:

    #include <ctl-parser/parser.h>

    Formula * parse_ctl_formula (const char *input, int debug);

The Formula pointer must be freed when no longer needed.
