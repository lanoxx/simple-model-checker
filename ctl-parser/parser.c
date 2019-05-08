#include <ctl-parser/parser.h>

#include <ctl-parser/heading.h>

#include <bison.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Global interface variable required by bison parser.
 *
 * This variable will contain the parsed result after
 * yyparse() returns. The output must be freed.
 */
Formula *root_formula;

/* Global interface variable required by flex lexer.
 *
 * The lexer will read its input from this string when
 * yyparse() is called.
 */
char *global_lexer_input;

/* global interface variable used by bison parser.
 *
 * When set to 1 debugging output will be enabled.
 */
int yydebug = 0;

Formula *
parse_ctl_formula (char *input, int debug)
{
  yydebug = debug;

  if (global_lexer_input) {
    perror("Global variable `global_lexer_input` must be initialized to NULL. "
           "Initialize `global_lexer_input` to NULL before calling parse_ctl_formula().\n");

    return NULL;
  }

  if (root_formula) {
    perror ("The global variable `root_formula` must be initialized to NULL. "
            "Initialize `root_formula` to NULL before calling parse_ctl_formula().\n");

    return NULL;
  }

  Formula *result;
  char *input_copy = malloc (strlen(input) + 1);
  strncpy (input_copy, input, strlen(input)+1);

  global_lexer_input = input_copy;

  yyparse();

  result = root_formula;

  free (input_copy);
  global_lexer_input = NULL;
  root_formula = NULL;

  return result;
}
