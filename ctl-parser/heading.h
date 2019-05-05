/* heading.h */

/**
 * _POSIX_C_SOURCE needed for fileno
 */
#define _POSIX_C_SOURCE 200809L
#define YY_NO_UNPUT
#define YY_NO_INPUT

#include<stdio.h>
#include<stdlib.h>
#include <string.h>

#include "formula.h"

/**
 * This is a global interface variable of our flex lexer.
 *
 * Users of the lexer must declare this variable and
 * initialize it to the input string that should be parsed.
 */
extern char *global_lexer_input;

/**
 * This is a global interface variable used by our bison parser.
 *
 * Users of the lexer must declare this variable and
 */
extern Formula *root_formula;
