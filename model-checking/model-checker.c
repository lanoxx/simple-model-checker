#include <model-checking/model-checker.h>

#include <stdio.h>

#include <ctl-parser/parser.h>

GList *model_check (KripkeStructure *kripkeStructure, Formula *formula)
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

GList *model_check_from_string (KripkeStructure *kripkeStructure,
                                const char *formula)
{
  Formula *ctl_formula = parse_ctl_formula (formula, FALSE);

  GList* result = model_check (kripkeStructure, ctl_formula);

  formula_free (ctl_formula);

  return result;

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

  GList *subformula_states = model_check (kripke, subformula);

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

  GList *left_states = model_check (kripke, subformula_left);
  GList *right_states = model_check (kripke, subformula_right);

  GList *result_states = NULL;

  GList *current_state_item = left_states;

  while (current_state_item)
  {

    State *current_state = current_state_item->data;

    current_state_item = g_list_next (current_state_item);

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

  GList *subformula_states = model_check (kripke, subformula);

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

  g_list_free (subformula_states);

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

  GList *states_left = model_check (kripke, subformula_left);
  GList *states_right = model_check (kripke, subformula_right);

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

  g_list_free (prev_result_states);

  return result_states;
}

State *
resultStateWithoutSuccessor (GList *result_states, GList *relations)
{
  GList *state_a_item = result_states;

  while (state_a_item) {

    State *state_a = state_a_item->data;

    gboolean has_successor = FALSE;

    GList *result_state_b_item = result_states;

    while (result_state_b_item && !has_successor) {

      State *state_b = result_state_b_item->data;

      GList *relation_item = relations;

      while (relation_item) {

        Relation *relation = relation_item->data;

        if (relation->state1 == state_a && relation->state2 == state_b) {
          has_successor = TRUE;
          break;
        }

        relation_item = g_list_next (relation_item);
      }

      result_state_b_item = g_list_next (result_state_b_item);
    }

    if (!has_successor) {
      return state_a;
    }

    state_a_item = g_list_next (state_a_item);
  }

  return NULL;
}

GList *
getStatesExistsGlobally (KripkeStructure *kripke, Formula *formula)
{
  Formula *subformula = formula->subformulas->data;

  GList *subformula_states = model_check (kripke, subformula);

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

  g_list_free (subformula_states);

  g_list_free (prev_result_states);

  return result_states;
}
