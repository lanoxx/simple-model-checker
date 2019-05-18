# About

This project implements a [marking algorithm](model-checking) used in
model checking of Kripke structures with a minimal set of CTL operators.

A small library is included for [reading Kripke structures](kripke)
from JSON files.

In addition a [CTL* parser library](ctl-parser) is available that can parse
CTL* formulas and convert them into an abstract syntax tree (AST) with a
minimal set of CTL operators.

# Requirements

To build this project the following libraries are required:

 * `libglib2.0-dev` (libglib2.0-0)
 * `libjson-glib-dev` (libjson-glib-1.0-0)

This project can be built using CMake 3.10 or later.

# Build

    mkdir build
    cd build
    cmake ..
    make

# Usage

    cd build
    ./model_checking --kripke-file=<some_json_file>.json --formula=<ctl-star-formula>

**Example:**

    ./model_checking --kripke-file=kripke.json --formula="AG(not a)"
