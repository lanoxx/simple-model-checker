## Kripke Structures Library

The Kripke structures library is implemented as a static library that must
be linked into the target program. The library provides a
constructor function which can deserialize a KripkeStructure from
a JSON encoded string or read it a Kripke structure directly from
a JSON file. 

The JSON must have the following format:

    {
      "states": [
        { "name": "<state name>", "labels": [ "<atom>", ... ] },
        ...
      ],
      "initialStates": "<state name>",
      "relations": [ 
        [ "<state name 1>", "<state name 2>"], 
        ...
      ]
    }

Kripke structures can be created and used via the following API:

    #include <kripke/kripke.h>

    KripkeStructure *kripke;



    // parse a KripkeStructure from a JSON encoded string
    KripkeStructure * kripke_structure_new_from_string (const char  *input,
                                                        GError     **error);

    // load a KripkeStructure from a JSON file
    KripkeStructure * kripke_structure_new_from_file (const char *filename,
                                                      GError    **error);

    // the KripkeStructure must be freed when no longer needed
    void kripke_free (KripkeStructure *kripke);

    // print the content of a Kripke strucutre. Useful for debugging.
    void kripke_print (KripkeStructure *kripke);
