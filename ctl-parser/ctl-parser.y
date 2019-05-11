/* Mini Calculator */
/* calc.y */

%{
#include <ctl-parser/heading.h>
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
%token	<formula>		EXISTS_FUTURE
%token	<formula>		ALWAYS_GLOBALLY
%token	<formula>		ALWAYS_FUTURE
%token	<formula>		ALWAYS_NEXT
%token	<formula>		ALWAYS
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
		| EXISTS_FUTURE '(' stmt ')'       {
		                                     Formula *true_formula =
		                                       formula_new_value (CTL_TYPE_BOOLEAN_LITERAL, TRUE);

		                                     Formula *stmt_formula =
		                                       formula_new_composite (CTL_TYPE_EXIST_UNTIL, "EU", true_formula, $3, NULL);

		                                     $$ = stmt_formula;
		                                   }
		| ALWAYS_NEXT '(' stmt ')'         {
		                                     // AX == not EX (not stmt)

		                                     Formula *not_stmt_formula =
		                                       formula_new_composite (CTL_TYPE_NEGATION, "negation", $3, NULL);

		                                     Formula *exists_next_formula =
		                                       formula_new_composite (CTL_TYPE_EXIST_NEXT, "EX", not_stmt_formula, NULL);

		                                     Formula *stmt_formula =
		                                       formula_new_composite (CTL_TYPE_NEGATION, "negation", exists_next_formula, NULL);

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
		| ALWAYS_FUTURE '(' stmt ')'       {
		                                     // AF == not EG (not stmt)

		                                     Formula *not_stmt_formula =
		                                       formula_new_composite (CTL_TYPE_NEGATION, "negation", $3, NULL);

		                                     Formula *eg_formula =
		                                       formula_new_composite (CTL_TYPE_EXISTS_GLOBALLY, "EG", not_stmt_formula, NULL);

		                                     Formula *not_eg_formula =
		                                      formula_new_composite (CTL_TYPE_NEGATION, "negation", eg_formula, NULL);

		                                     $$ = not_eg_formula;
		                                   }
		| EXISTS stmt UNTIL stmt           {
		                                     $$ = formula_new_composite (CTL_TYPE_EXIST_UNTIL, "EU", $2, $4, NULL);
		                                   }
		| ALWAYS stmt UNTIL stmt           {
		                                     // A stmt U stmt = not E[(not stmt2) U (not stmt1 and not stmt2))] and not EG(not stmt2)

		                                     Formula * not_stmt1 =
		                                        formula_new_composite (CTL_TYPE_NEGATION, "negation", $2, NULL);

		                                     Formula * not_stmt2 =
		                                        formula_new_composite (CTL_TYPE_NEGATION, "negation", $4, NULL);

		                                     Formula * and_formula =
		                                       formula_new_composite (CTL_TYPE_CONJUNCTION, "and", not_stmt1, not_stmt1, NULL);

		                                     Formula * eu_formula =
		                                       formula_new_composite (CTL_TYPE_EXIST_UNTIL, "EU", not_stmt2, and_formula, NULL);

		                                     Formula * eg_formula =
		                                       formula_new_composite (CTL_TYPE_EXISTS_GLOBALLY, "EG", not_stmt2, NULL);

		                                     Formula * not_eu_formula =
		                                       formula_new_composite (CTL_TYPE_NEGATION, "negation", eu_formula, NULL);

		                                     Formula * not_eg_formula =
		                                       formula_new_composite (CTL_TYPE_NEGATION, "negation", eg_formula, NULL);

		                                     Formula * stmt_formula =
		                                       formula_new_composite (CTL_TYPE_CONJUNCTION, "and", not_eu_formula, not_eg_formula, NULL);

		                                     $$ = stmt_formula;
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
