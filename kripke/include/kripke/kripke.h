#ifndef KRIPKE_H
#define KRIPKE_H

#include <glib.h>

typedef struct state
{
  char *name;
} State;

typedef struct relation
{
  State *state1;
  State *state2;
} Relation;

typedef struct label
{
  State *state;
  char* atom;
} Label;

typedef struct kripkeStructure
{
  GList *states;       /* GList<State> states */
  State *initialState;
  GList *relations;    /* GList<Relation> *relations */
  GList *labels;       /* GList<Label> *labels */
} KripkeStructure;

KripkeStructure * kripke_structure_new (GList * states,
                                        State *initial_state,
                                        GList *relations,
                                        GList *labels);

void kripke_print_states (GList *states);

#endif /* KRIPKE_H */
