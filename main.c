#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <glib.h>

#include <ctl-parser/formula.h>
#include <ctl-parser/parser.h>

#include <kripke/kripke.h>

#include <model-checking/model-checker.h>

int main ()
{
  KripkeStructure *kripke;
  GError *error = NULL;

  char *kripke_json = "{\n"
                      "  \"states\": [\n"
                      "    { \"name\": \"s0\", \"labels\": [ \"a\", \"bar\" ] },\n"
                      "    { \"name\": \"s1\", \"labels\": [ \"a\" ] },\n"
                      "    { \"name\": \"s2\", \"labels\": [ \"bar\" ] },\n"
                      "    { \"name\": \"s3\", \"labels\": [ \"a\", \"bar\" ] }\n"
                      "  ],\n"
                      "  \"initialStates\": [ \"s0\" ],\n"
                      "  \"relations\": [ [ \"s0\", \"s1\" ], [ \"s1\", \"s2\" ], [ \"s1\", \"s0\" ], [ \"s2\", \"s3\" ], [ \"s2\", \"s0\" ], [ \"s3\", \"s3\" ] ]\n"
                      "}";

  kripke = kripke_structure_new_from_string (kripke_json, &error);

  printf ("Checking the following Kripke structure:\n");
  kripke_print (kripke);
  printf("\n");

  printf ("[CTL-PARSER]Result states EX(bar):\n");
  GList *result_states_EX = model_check_from_string (kripke, "EX(bar)");
  kripke_print_states (result_states_EX);
  printf("\n");

  printf ("[CTL-PARSER]Result states EG(bar):\n");
  GList *result_states_EG = model_check_from_string (kripke, "EG(bar)");
  kripke_print_states (result_states_EG);
  printf("\n");

  printf ("[CTL-PARSER]Result states (a U bar)\n");
  GList *result_states_a_U_b = model_check_from_string (kripke, "E a U bar");
  kripke_print_states (result_states_a_U_b);
  printf("\n");

  printf ("[CTL-PARSER] Result states (bar U a)\n");
  GList *result_states_b_U_a = model_check_from_string (kripke, "E bar U a");
  kripke_print_states (result_states_b_U_a);
  printf ("\n");

  printf ("[CTL-PARSER] Result states 'not bar'\n");
  GList *parsed_result_states_not_b = model_check_from_string (kripke, "not bar");
  kripke_print_states (parsed_result_states_not_b);

  printf ("\n");

  printf ("[CTL-PARSER] Result states AF(bar)\n");
  GList *result_states_AF_b = model_check_from_string (kripke, "AF(bar)");
  kripke_print_states (result_states_AF_b);
  printf("\n");

  printf("[CTL-PARSER] Result states 'AG(bar)'\n");
  GList *parsed_result_states_ag_b = model_check_from_string (kripke, "AG(bar)");
  kripke_print_states (parsed_result_states_ag_b);
  printf("\n");

  return EXIT_SUCCESS;
}
