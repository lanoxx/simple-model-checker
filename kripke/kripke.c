#include <kripke/kripke.h>

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
