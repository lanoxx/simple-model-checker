#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <glib.h>

#include <ctl-parser/formula.h>
#include <ctl-parser/parser.h>

#include <kripke/kripke.h>

#include <model-checking/model-checker.h>

void
print_states (GList *states);

int main ()
{
  State state_0, state_1, state_2, state_3;
  Label label_s0_a, label_s0_b, label_s1_a, label_s2_b, label_s3_a, label_s3_b;
  Relation relation_s0s1, relation_s1s0, relation_s1s2, relation_s2s0, relation_s2s3, relation_s3s3;

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
  label_s0_b.atom = strdup("bar");

  label_s1_a.state = &state_1;
  label_s1_a.atom = strdup("a");

  label_s2_b.state = &state_2;
  label_s2_b.atom = strdup("bar");

  label_s3_a.state = &state_3;
  label_s3_a.atom = strdup("a");

  label_s3_b.state = &state_3;
  label_s3_b.atom = strdup("bar");

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

  printf ("[CTL-PARSER]Result states EX(bar):\n");
  GList *result_states_EX = model_check_from_string (&kripke, "EX(bar)");
  print_states (result_states_EX);
  printf("\n");

  printf ("[CTL-PARSER]Result states EG(bar):\n");
  GList *result_states_EG = model_check_from_string (&kripke, "EG(bar)");
  print_states (result_states_EG);
  printf("\n");

  printf ("[CTL-PARSER]Result states (a U bar)\n");
  GList *result_states_a_U_b = model_check_from_string (&kripke, "E a U bar");
  print_states (result_states_a_U_b);
  printf("\n");

  printf ("[CTL-PARSER] Result states (bar U a)\n");
  GList *result_states_b_U_a = model_check_from_string (&kripke, "E bar U a");
  print_states (result_states_b_U_a);
  printf ("\n");

  printf ("[CTL-PARSER] Result states 'not bar'\n");
  GList *parsed_result_states_not_b = model_check_from_string (&kripke, "not bar");
  print_states (parsed_result_states_not_b);

  printf ("\n");

  printf ("[CTL-PARSER] Result states AF(bar)\n");
  GList *result_states_AF_b = model_check_from_string (&kripke, "AF(bar)");
  print_states (result_states_AF_b);
  printf("\n");

  printf("[CTL-PARSER] Result states 'AG(bar)'\n");
  GList *parsed_result_states_ag_b = model_check_from_string (&kripke, "AG(bar)");
  print_states (parsed_result_states_ag_b);
  printf("\n");

  return EXIT_SUCCESS;
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
