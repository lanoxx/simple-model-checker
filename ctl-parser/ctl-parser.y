/* Mini Calculator */
/* calc.y */

%{
#include <heading.h>
#include <glib.h>

/**
 * This activates debugging in the built binary, but to actually get
 * debugging output one needs to declare a global variable `int yydebug;`
 * and define it to 1 `yydebug=1` in main.
 */
#define YYDEBUG 1
int yyerror(const char *s);
int yylex(void);
%}

%define parse.error verbose
%define parse.trace
%define parse.lac full
%define lr.type ielr

%union{
  char*	op_val;
  char*	atom_name;
  struct formula *formula;
}

%start	input

%token	<atom_name>		ATOMIC_PROPOSITION
%token	<formula>		EXISTS_NEXT
%token	<formula>		EXISTS_GLOBALLY
%token	<formula>		ALWAYS_GLOBALLY
%token	<formula>		EXISTS
%token	<formula>		UNTIL
%token	<formula>		NOT
%token	<atom_name>		BOOLEAN_LITERAL

%type	<formula>		stmt
%type	<formula>		atom
%type	<formula>		boolean

%left   <formula>		AND
%nonassoc '('
%nonassoc ')'

%%

input:  %empty
		| stmt                             {
		                                     root_formula = $1; YYACCEPT;
		                                   }
		;

stmt:   atom                               {
		                                     $$ = $1;
		                                   }
		| boolean                          {
		                                     $$ = $1;
		                                   }
		| NOT atom                         {
		                                     Formula *stmt_formula =
		                                       formula_new_composite (CTL_TYPE_NEGATION, "negation", $2, NULL);

		                                     $$ = stmt_formula;
		                                   }
		| EXISTS_NEXT '(' stmt ')'         {
		                                     Formula *stmt_formula =
		                                       formula_new_composite (CTL_TYPE_EXIST_NEXT, "EX", $3, NULL);

		                                     $$ = stmt_formula;
		                                   }
		| EXISTS_GLOBALLY '(' stmt ')'     {
		                                     Formula *stmt_formula =
		                                      formula_new_composite (CTL_TYPE_EXISTS_GLOBALLY, "EG", $3, NULL);

		                                     $$ = stmt_formula;
		                                   }
		| ALWAYS_GLOBALLY '(' stmt ')'     {
		                                     // AG == not E (true U (not stmt))

		                                     Formula *not_stmt_formula =
		                                       formula_new_composite (CTL_TYPE_NEGATION, "negation", $3, NULL);

		                                     Formula *true_formula =
		                                       formula_new_value (CTL_TYPE_BOOLEAN_LITERAL, TRUE);

		                                     Formula *until_formula =
		                                       formula_new_composite (CTL_TYPE_EXIST_UNTIL, "EU", true_formula, not_stmt_formula, NULL);

		                                     Formula *not_exists_formula =
		                                      formula_new_composite (CTL_TYPE_NEGATION, "negation", until_formula, NULL);

		                                     $$ = not_exists_formula;
		                                   }
		| EXISTS stmt UNTIL stmt           {
		                                     $$ = formula_new_composite (CTL_TYPE_EXIST_UNTIL, "EU", $2, $4, NULL);
		                                   }
		| stmt AND stmt                    {
		                                     $$ = formula_new_composite(CTL_TYPE_CONJUNCTION, "and", $1, $3, NULL);
		                                   }
		;

atom:	ATOMIC_PROPOSITION                 {
		                                     $$ = formula_new_name (CTL_TYPE_ATOMIC_PROPOSITION, $1);
		                                   }

boolean: BOOLEAN_LITERAL                   {
		                                     gboolean value;

		                                     if (g_strcmp0($1, "true") == 0) {
		                                       value = TRUE;
		                                     } else {
		                                       value = FALSE;
		                                     }

		                                     $$ = formula_new_value (CTL_TYPE_BOOLEAN_LITERAL, value);
		                                   }
		;

%%

int yyerror(const char *s)
{
  extern int yylineno;	// defined and maintained in lex.c
  extern char *yytext;	// defined and maintained in lex.c

  printf ("ERROR: %s at symbol \"%s", s, yytext);
  printf ("\" on line %d", yylineno);
  exit(1);
}
