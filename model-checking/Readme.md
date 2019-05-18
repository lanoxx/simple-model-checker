## Model Checker Library

The model checker library implements the marking algorithm, that
does the actual checking of a KripkeStructure against a CTL* formula.

The `model_check_from_string` function accepts a `KripkeStructure` and
a CTL formula represented as a string (or optionally as a parse tree
(see `Formula` in ctl-parser library)) and returns a list of states
that satisfy the CTL* formula.

The model checker can be used via the following API:

    #include <model-checking/model-checker.h>

    // Check kripke structure against formula and returns a list of State
    // objects. The returned list must be freed with g_list_free when no
    // longer needed.
    GList *model_check_from_string (KripkeStructure *kripkeStructure,
                                    const char *formula);

**Example:**

    KripkeStructure kripke;

    // define kripke->states;
    // define kripke->labels;
    // define kripke->relations;

    GList *result_states = model_check_from_string (&kripke, "EG(bar)");
    kripke_print_states (result_states);
    g_list_free (result_states);
