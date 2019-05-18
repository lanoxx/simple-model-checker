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

KripkeStructure * kripke_structure_new_from_file (const char *filename,
                                                  GError    **error);

KripkeStructure * kripke_structure_new_from_string (const char  *input,
                                                    GError     **error);

void kripke_print (KripkeStructure *kripke);

void kripke_free (KripkeStructure *kripke);
void kripke_state_free  (State *state);
void kripke_label_free  (Label *label);
void kripke_relation_free (Relation *relation);
void kripke_states_free (GList *list);
void kripke_labels_free (GList *list);
void kripke_relations_free (GList *list);

void kripke_print_states (GList *states);

#endif /* KRIPKE_H */
