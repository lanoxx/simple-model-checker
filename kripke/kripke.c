#include <kripke/kripke.h>

#include <stdio.h>

KripkeStructure * kripke_structure_new (GList * states,
                                        State *initial_state,
                                        GList *relations,
                                        GList *labels)
{
  KripkeStructure *kripke = malloc (sizeof (KripkeStructure));

  kripke->states = states;
  kripke->relations = relations;
  kripke->initialState = initial_state;
  kripke->labels = labels;

  return kripke;
}

/**
 * Prints each state in the given list to stdout.
 *
 * @param states A pointer to a GList<State> of State structures.
 */
void kripke_print_states (GList *states)
{
  GList *state_item = states;

  while (state_item) {
    State *result_state = state_item->data;

    printf ("state: %s\n", result_state->name);

    state_item = g_list_next (state_item);
  }
}
