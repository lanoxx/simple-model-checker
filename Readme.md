# About

This project implements a [marking algorithm](model-checking) used in
model checking of Kripke structures with a minimal set of CTL operators.

A small library is included for [reading Kripke structures](kripke)
from JSON files.

In addition a [CTL* parser library](ctl-parser) is available that can parse
CTL* formulas and convert them into an abstract syntax tree (AST) with a
minimal set of CTL operators.

# Usage

## Build

    mkdir build
    cd build
    cmake ..
    make

## Run

    cd build
    ./model_checking --kripke-file=<some_json_file>.json --formula=<ctl-star-formula>

## Example

    ./model_checking --kripke-file=kripke.json --formula="AG(not a)"
