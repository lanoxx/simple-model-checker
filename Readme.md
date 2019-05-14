# About

This project implements a marking algorithm used in model checking of
Kripke structures with a minimal set of CTL operators.

In addition a CTL parser library is available that can parse CTL* formulas
and convert them into a minimal set of CTL operators.

## CTL-Parser

The CTL-Parser can parse CTL* formulas and returns an abstract syntax
tree that represents a minimal set of CTL operators consisting of the following
statements:

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

The CTL-Parser is implemented as a static library and must be linked
into the target program.

The parser can be used via the following API:

    #include <ctl-parser/parser.h>

    Formula * parse_ctl_formula (char *input, int debug);

## Kripke Structures

Kripke structures are implemented as a static library and must also
be linked into the target program. The Kripke library provides a
constructor function which can deserialize a KripkeStructure from
a JSON encoded string (see below). The JSON must have the following
format:

    {
      "states": [ { "name": "<state name>", "labels": [ "<atom>", ... ] ],
      "initialStates": "<state name>",
      "relations:" [ [ "<state name 1>", "<state name 2>"], ... ]
    }

Kripke structures can be used via the following API:

    #include <kripke/kripke.h>

    KripkeStructure kripke;

    void kripke_print_states (GList *states);

    // parse a KripkeStructure from a JSON encoded string
    KripkeStructure * kripke_structure_new_from_string (const char  *input,
                                                        GError     **error);

## Model Checker

The model checker accepts a `KripkeStructure` and a CTL formula represented as
a string (or optionally as a parse tree (see `Formula` in ctl-parser library)).

The model checker can be used via the following API:

    #include <model-checking/model-checker.h>

    GList * model_check_from_string (KripkeStructure *kripke, char * input);

**Example:**

    KripkeStructure kripke;

    // define kripke->states;
    // define kripke->labels;
    // define kripke->relations;

    GList *result_states = model_check_from_string (&kripke, "EG(bar)");
    kripke_print_states (result_states);

# Usage

## Build

    mkdir build
    cd build
    cmake ..
    make

## Run

    cd build
    ./model_checking <some_json_file>.json
