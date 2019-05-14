#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <glib.h>
#include <gio/gio.h>

#include <ctl-parser/formula.h>
#include <ctl-parser/parser.h>

#include <kripke/kripke.h>

#include <model-checking/model-checker.h>

int main (int argc, char *argv[])
{
  KripkeStructure *kripke;
  GError *error = NULL;

  const char* input_filename = NULL;

  if (argc > 1)
  {
    input_filename = argv[1];

    printf ("input file name: %s\n", input_filename);
  }

  GFile *input_file = g_file_new_for_path (input_filename);
  GError *load_error = NULL;

  char *content = NULL;

  gboolean loaded =
      g_file_load_contents (input_file,
                            NULL,
                            &content,
                            NULL,
                            NULL,
                            &load_error);

  if (!loaded)
  {
    printf ("Could not read input file: %s\n", input_filename);

    return EXIT_FAILURE;
  }

  kripke = kripke_structure_new_from_string (content, &error);

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

  printf("[CTL-PARSER] Result states 'EF(not bar)'\n");
  GList *parsed_result_states_af_not_b = model_check_from_string (kripke, "EF(not bar)");
  kripke_print_states (parsed_result_states_af_not_b);
  printf("\n");

  printf("[CTL-PARSER] Result states 'AF(not bar)'\n");
  GList *parsed_result_states_ef_not_b = model_check_from_string (kripke, "AF(not bar)");
  kripke_print_states (parsed_result_states_ef_not_b);
  printf("\n");

  return EXIT_SUCCESS;
}
