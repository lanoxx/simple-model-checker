#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <glib.h>

#include "formula.h"

#include <parser.h>

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

GList *
getStatesBooleanLiteral (KripkeStructure *kripke, Formula *formula);

GList *
getStatesWithAtomicProposition (KripkeStructure *kripke, Formula *formula);

GList *
getStatesWithNegation (KripkeStructure *kripke, Formula *formula);

GList *
getStatesWithConjunction (KripkeStructure *kripke, Formula *formula);

GList *
getStatesExistsNext (KripkeStructure *kripke, Formula *formula);

GList *
getStatesExistsUntil (KripkeStructure *kripke, Formula *formula);

GList *
getStatesExistsGlobally (KripkeStructure *kripke, Formula *formula);

/**
 *
 * @param kripkeStructure
 * @param formula
 * @return A list of states (GList<State> *).
 */
GList *modelCheck (KripkeStructure *kripkeStructure,
                   Formula *formula);

void
print_states (GList *states);

int main ()
{
  State state_0, state_1, state_2, state_3;
  Label label_s0_a, label_s0_b, label_s1_a, label_s2_b, label_s3_a, label_s3_b;
  Relation relation_s0s1, relation_s1s0, relation_s1s2, relation_s2s0, relation_s2s3, relation_s3s3;

  Formula formula_EX;
  Formula formula_b, formula_a;
  formula_EX.type = CTL_TYPE_EXIST_NEXT;
  formula_EX.subformulas = g_list_append (NULL, &formula_b);

  formula_a.type = CTL_TYPE_ATOMIC_PROPOSITION;
  formula_a.subformulas = NULL;
  formula_a.name = strdup ("a");

  formula_b.type = CTL_TYPE_ATOMIC_PROPOSITION;
  formula_b.subformulas = NULL;
  formula_b.name = strdup ("b");

  Formula formula_EG;
  formula_EG.type = CTL_TYPE_EXISTS_GLOBALLY;
  formula_EG.subformulas = g_list_append (NULL, &formula_b);

  Formula formula_a_U_b;
  formula_a_U_b.type = CTL_TYPE_EXIST_UNTIL;
  formula_a_U_b.subformulas = g_list_append(g_list_append (NULL, &formula_a), &formula_b);

  Formula formula_b_U_a;
  formula_b_U_a.type = CTL_TYPE_EXIST_UNTIL;
  formula_b_U_a.subformulas = g_list_append(g_list_append (NULL, &formula_b), &formula_a);

  Formula formula_not_b;
  formula_not_b.type = CTL_TYPE_NEGATION;
  formula_not_b.subformulas = g_list_append(NULL, &formula_b);

  // AF_b:
  Formula formula_EG_not_b;

  Formula formula_not_EG_not_b;
  formula_not_EG_not_b.type = CTL_TYPE_NEGATION;
  formula_not_EG_not_b.subformulas = g_list_append (NULL, &formula_EG_not_b);

  formula_EG_not_b.type = CTL_TYPE_EXISTS_GLOBALLY;
  formula_EG_not_b.subformulas = g_list_append (NULL, &formula_not_b);

  GList *states = NULL;

  state_0.name = "s0";
  state_1.name = "s1";
  state_2.name = "s2";
  state_3.name = "s3";

  states = g_list_append (states, &state_0);
  states = g_list_append (states, &state_1);
  states = g_list_append (states, &state_2);
  states = g_list_append (states, &state_3);

  GList *labels = NULL;

  label_s0_a.state = &state_0;
  label_s0_a.atom = strdup ("a");

  label_s0_b.state = &state_0;
  label_s0_b.atom = strdup("b");

  label_s1_a.state = &state_1;
  label_s1_a.atom = strdup("a");

  label_s2_b.state = &state_2;
  label_s2_b.atom = strdup("b");

  label_s3_a.state = &state_3;
  label_s3_a.atom = strdup("a");

  label_s3_b.state = &state_3;
  label_s3_b.atom = strdup("b");

  labels = g_list_append (labels, &label_s0_a);
  labels = g_list_append (labels, &label_s0_b);
  labels = g_list_append (labels, &label_s1_a);
  labels = g_list_append (labels, &label_s2_b);
  labels = g_list_append (labels, &label_s3_a);
  labels = g_list_append (labels, &label_s3_b);

  GList *relations = NULL;

  relation_s0s1.state1 = &state_0;
  relation_s0s1.state2 = &state_1;

  relation_s1s0.state1 = &state_1;
  relation_s1s0.state2 = &state_0;

  relation_s1s2.state1 = &state_1;
  relation_s1s2.state2 = &state_2;

  relation_s2s0.state1 = &state_2;
  relation_s2s0.state2 = &state_0;

  relation_s2s3.state1 = &state_2;
  relation_s2s3.state2 = &state_3;

  relation_s3s3.state1 = &state_3;
  relation_s3s3.state2 = &state_3;

  relations = g_list_append (relations, &relation_s0s1);
  relations = g_list_append (relations, &relation_s1s0);
  relations = g_list_append (relations, &relation_s1s2);
  relations = g_list_append (relations, &relation_s2s0);
  relations = g_list_append (relations, &relation_s2s3);
  relations = g_list_append (relations, &relation_s3s3);

  KripkeStructure kripke;

  kripke.states = states;
  kripke.relations = relations;
  kripke.initialState = &state_0;
  kripke.labels = labels;

  GList *result_states_EX = modelCheck (&kripke, &formula_EX);

  printf ("Result states EX(b):\n");
  print_states (result_states_EX);

  printf("\n");

  GList *result_states_EG = modelCheck (&kripke, &formula_EG);

  printf ("Result states EG(b):\n");
  print_states (result_states_EG);

  printf("\n");

  printf ("Result states (a U b)\n");
  GList *result_states_a_U_b = modelCheck (&kripke, &formula_a_U_b);
  print_states (result_states_a_U_b);

  printf("\n");

  printf ("Result states (b U a)\n");
  GList *result_states_b_U_a = modelCheck (&kripke, &formula_b_U_a);
  print_states (result_states_b_U_a);

  printf("\n");

  printf ("Result states not(b)\n");
  GList *result_states_not_b = modelCheck (&kripke, &formula_not_b);
  print_states (result_states_not_b);

  printf ("\n");

  printf ("Result states AF(b)\n");
  GList *result_states_AF_b = modelCheck (&kripke, &formula_not_EG_not_b);
  print_states (result_states_AF_b);

  printf ("\n");

  printf ("[CTL-PARSER] Result states 'not b'\n");

  Formula *formula_not_b_parsed = parse_ctl_formula ("not b", 0);
  GList *parsed_result_states_not_b = modelCheck (&kripke, formula_not_b_parsed);
  print_states (parsed_result_states_not_b);

  printf("\n");

  printf("[CTL-PARSER] Result states 'AG(b)'\n");

  Formula *formula_ag_b_parsed = parse_ctl_formula ("AG(b)", 0);
  GList *parsed_result_states_ag_b = modelCheck (&kripke, formula_ag_b_parsed);
  print_states (parsed_result_states_ag_b);

  printf("\n");

  return EXIT_SUCCESS;
}

GList *
getStatesBooleanLiteral (KripkeStructure *kripke, Formula *formula)
{
  if (formula->value == TRUE)
  {
    return g_list_copy (kripke->states);
  } else
  {
    return NULL;
  }
}

GList *
getStatesWithAtomicProposition (KripkeStructure *kripke, Formula *formula)
{
  char *atom = formula->name;

  GList *current_state_item = kripke->states;

  GList *result_states = NULL;

  while (current_state_item)
  {

    State *current_state = current_state_item->data;

    GList *current_label_item = kripke->labels;

    while (current_label_item)
    {

      Label *current_label = current_label_item->data;

      gboolean atom_names_equal = g_strcmp0 (current_label->atom,atom) == 0;

      if (current_label->state == current_state_item->data &&
          atom_names_equal)
      {
        result_states = g_list_append (result_states, current_state);
        break;
      }

      current_label_item = g_list_next (current_label_item);
    }

    current_state_item = g_list_next (current_state_item);
  }

  return result_states;
}

GList *
getStatesWithNegation (KripkeStructure *kripke, Formula *formula)
{
  Formula *subformula = formula->subformulas->data;

  GList *subformula_states = modelCheck (kripke, subformula);

  GList *result_states = NULL;

  GList *current_state_item = kripke->states;

  while (current_state_item)
  {
    State *current_state = current_state_item->data;

    GList *found = g_list_find (subformula_states, current_state);

    if (!found)
    {
      result_states = g_list_append (result_states, current_state);
    }

    current_state_item = g_list_next (current_state_item);
  }

  g_list_free (subformula_states);

  return result_states;
}

GList *
getStatesWithConjunction (KripkeStructure *kripke, Formula *formula)
{
  Formula *subformula_left = formula->subformulas->data;
  Formula *subformula_right = formula->subformulas->next->data;

  GList *left_states = modelCheck (kripke, subformula_left);
  GList *right_states = modelCheck (kripke, subformula_right);

  GList *result_states = NULL;

  GList *current_state_item = left_states;

  while (current_state_item)
  {

    State *current_state = current_state_item->data;

    GList *found = g_list_find (right_states, current_state);

    if (!found)
    {
      continue;
    }

    result_states = g_list_append (result_states, current_state);
  }

  g_list_free (left_states);
  g_list_free (right_states);

  return result_states;
}

gboolean
statePreceedsSubformulaState (State *state, GList *subformula_states, GList *relations)
{
  GList *subformula_state_item = subformula_states;

  while (subformula_state_item) {

    State *subformula_state = subformula_state_item->data;

    GList *current_relation_item = relations;

    while (current_relation_item) {

      Relation *relation = current_relation_item->data;

      gboolean state_preceeds_subformula_state =
          relation->state1 == state && relation->state2 == subformula_state;

      if (state_preceeds_subformula_state) {
        return TRUE;
      }

      current_relation_item = g_list_next (current_relation_item);
    }

    subformula_state_item = g_list_next (subformula_state_item);
  }

  return FALSE;
}

GList *
getStatesExistsNext (KripkeStructure *kripke, Formula *formula)
{
  Formula *subformula = formula->subformulas->data;

  GList *subformula_states = modelCheck (kripke, subformula);

  GList *result_states = NULL;

  GList *state_item = kripke->states;

  while (state_item) {

    State *state = state_item->data;

    gboolean state_preceeeds_subformula_state =
        statePreceedsSubformulaState (state, subformula_states, kripke->relations);

    if (state_preceeeds_subformula_state) {
      result_states = g_list_append (result_states, state);
    }

    state_item = g_list_next (state_item);
  }

  return result_states;
}

gboolean lists_compare (GList *a, GList* b)
{
  if (g_list_length (a) != g_list_length (b)) {
    return FALSE;
  }

  GList *current_item_a = a;

  while (current_item_a) {
    if (g_list_find (b, current_item_a->data) == NULL) {
      return FALSE;
    }

    current_item_a = g_list_next (current_item_a);
  }

  return TRUE;
}

GList *
getStatesExistsUntil (KripkeStructure *kripke, Formula *formula)
{
  Formula *subformula_left = formula->subformulas->data;
  Formula *subformula_right = formula->subformulas->next->data;

  GList *states_left = modelCheck (kripke, subformula_left);
  GList *states_right = modelCheck (kripke, subformula_right);

  GList *result_states = g_list_copy (states_right);
  GList *prev_result_states = NULL;

  gboolean lists_match = lists_compare (result_states, prev_result_states);

  while (!lists_match) {
    g_list_free (prev_result_states);

    prev_result_states = g_list_copy (result_states);

    GList *state_left_item = states_left;

    while (state_left_item) {

      State *state_left = state_left_item->data;

      gboolean state_has_successor_in_result_states =
          statePreceedsSubformulaState (state_left, result_states, kripke->relations);

      if (state_has_successor_in_result_states && g_list_find (result_states, state_left) == NULL) {
        result_states = g_list_append (result_states, state_left);
      }

      state_left_item = g_list_next (state_left_item);
    }

    lists_match = lists_compare (result_states, prev_result_states);
  }

  g_list_free (states_left);
  g_list_free (states_right);

  return result_states;
}

State *
resultStateWithoutSuccessor (GList *result_states, GList *relations)
{
  GList *state_a_item = result_states;

  while (state_a_item) {

    State *state_a = state_a_item->data;

    GList *result_state_b_item = result_states;

    while (result_state_b_item) {

      State *state_b = result_state_b_item->data;

      GList *relation_item = relations;

      while (relation_item) {

        Relation *relation = relation_item->data;

        if (relation->state1 == state_a && relation->state2 == state_b) {
          return state_a;
        }

        relation_item = g_list_next (relation_item);
      }

      result_state_b_item = g_list_next (result_state_b_item);
    }

    state_a_item = g_list_next (state_a_item);
  }

  return NULL;
}

GList *
getStatesExistsGlobally (KripkeStructure *kripke, Formula *formula)
{
  Formula *subformula = formula->subformulas->data;

  GList *subformula_states = modelCheck (kripke, subformula);

  GList *result_states = g_list_copy (subformula_states);
  GList *prev_result_states = NULL;

  gboolean lists_match = lists_compare (result_states, prev_result_states);

  while (!lists_match) {

    g_list_free (prev_result_states);

    prev_result_states = g_list_copy (result_states);

    State *state = resultStateWithoutSuccessor (result_states, kripke->relations);

    result_states = g_list_remove (result_states, state);

    lists_match = lists_compare (result_states, prev_result_states);
  }

  return result_states;
}

GList *modelCheck (KripkeStructure *kripkeStructure, Formula *formula)
{
  switch (formula->type)
  {
    case CTL_TYPE_NONE:
      printf ("invalid formula");
      return NULL;
    case CTL_TYPE_BOOLEAN_LITERAL:
    {
      GList *states = getStatesBooleanLiteral (kripkeStructure, formula);
      return states;
    }
    case CTL_TYPE_ATOMIC_PROPOSITION:
    {
      GList *states = getStatesWithAtomicProposition (kripkeStructure, formula);
      return states;
    }
    case CTL_TYPE_NEGATION:
    {
      GList *states = getStatesWithNegation (kripkeStructure, formula);
      return states;
    }
    case CTL_TYPE_CONJUNCTION:
    {
      GList *states = getStatesWithConjunction (kripkeStructure, formula);
      return states;
    }
    case CTL_TYPE_EXIST_NEXT:
    {
      GList *states = getStatesExistsNext (kripkeStructure, formula);
      return states;
    }
    case CTL_TYPE_EXIST_UNTIL:
    {
      GList *states = getStatesExistsUntil (kripkeStructure, formula);
      return states;
    }
    case CTL_TYPE_EXISTS_GLOBALLY:
    {
      GList *states = getStatesExistsGlobally (kripkeStructure, formula);
      return states;
    }
    default:
      printf ("default case reached. case not implemented.");
      return NULL;
  }
}

void print_states (GList *states)
{
  GList *state_item = states;

  while (state_item) {
    State *result_state = state_item->data;

    printf ("state: %s\n", result_state->name);

    state_item = g_list_next (state_item);
  }
}
