# About

This project implements a marking algorithm used in model checking of
Kripke structures with a minimal set of CTL operators.

## CTL-Parser

The CTL-Parser can parse CTL* formulas and returns an abstract syntax
tree that represents a minimal set of CTL operators consisting of the following
statements:

 * boolean literal (`true`, `false`) is a statement.
 * atoms (`[a-z]` are statements.
 * negations `not <statement>` are statements.
 * conjunctions `and <statement>` are statements.
 * EX(<statement>) is a statement.
 * EG(<statement>) is a statement.
 * E <statement> U <statement> is a statement.

# Using

