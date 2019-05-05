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
		                                     Formula *stmt_formula = formula_new();
		                                     stmt_formula->type = CTL_TYPE_NEGATION;
		                                     stmt_formula->subformulas = g_list_append (NULL, $2);
		                                     stmt_formula->name = strdup("negation");
		                                     $$ = stmt_formula;
		                                   }
		| EXISTS_NEXT '(' stmt ')'         {
		                                     Formula *stmt_formula = formula_new();
		                                     stmt_formula->type = CTL_TYPE_EXIST_NEXT;
		                                     stmt_formula->subformulas = g_list_append (NULL, $3);
		                                     stmt_formula->name = strdup("EX");
		                                     $$ = stmt_formula;
		                                   }
		| EXISTS_GLOBALLY '(' stmt ')'     {
		                                     Formula *stmt_formula = formula_new();
		                                     stmt_formula->type = CTL_TYPE_EXISTS_GLOBALLY;
		                                     stmt_formula->subformulas = g_list_append (NULL, $3);
		                                     stmt_formula->name = strdup("EG");
		                                     $$ = stmt_formula;
		                                   }
		| ALWAYS_GLOBALLY '(' stmt ')'     {
		                                     // not E (true U (not stmt))
		                                     Formula *not_stmt_formula = formula_new();
		                                     not_stmt_formula->type = CTL_TYPE_NEGATION;
		                                     not_stmt_formula->subformulas = g_list_append (NULL, $3);
		                                     not_stmt_formula->name = strdup("negation");

		                                     Formula *true_formula = formula_new();
		                                     true_formula->type = CTL_TYPE_BOOLEAN_LITERAL;
		                                     true_formula->subformulas = NULL;
		                                     true_formula->name = strdup("true");
		                                     true_formula->value = 1;

		                                     Formula *until_formula = formula_new();
		                                     until_formula->type = CTL_TYPE_EXIST_UNTIL;
		                                     GList *subformulas;
		                                     subformulas = g_list_append (NULL, true_formula);
		                                     subformulas = g_list_append (subformulas, not_stmt_formula);
		                                     until_formula->subformulas = subformulas;
		                                     until_formula->name = strdup("EU");

		                                     Formula *not_exists_formula = formula_new();
		                                     not_exists_formula->type = CTL_TYPE_NEGATION;
		                                     not_exists_formula->subformulas = g_list_append (NULL, until_formula);
		                                     not_exists_formula->name = strdup("negation");

		                                     $$ = not_exists_formula;
		                                   }
		| EXISTS stmt UNTIL stmt           {
		                                     Formula *stmt_formula = formula_new();
		                                     stmt_formula->type = CTL_TYPE_EXIST_UNTIL;
		                                     GList *subformulas;
		                                     subformulas = g_list_append (NULL, $2);
		                                     subformulas = g_list_append (subformulas, $4);
		                                     stmt_formula->subformulas = subformulas;
		                                     stmt_formula->name = strdup("EU");
		                                     $$ = stmt_formula;
		                                   }
		| stmt AND stmt                    {
		                                     Formula *stmt_formula = formula_new();
		                                     stmt_formula->type = CTL_TYPE_CONJUNCTION;
		                                     GList *subformulas;
                                             subformulas = g_list_append (NULL, $1);
                                             subformulas = g_list_append (subformulas, $3);
		                                     stmt_formula->subformulas = subformulas;
		                                     stmt_formula->name = strdup("and");
		                                     $$ = stmt_formula;
		                                   }
		;

atom:	ATOMIC_PROPOSITION                 {
		                                     Formula *stmt_formula = formula_new();
		                                     stmt_formula->type = CTL_TYPE_ATOMIC_PROPOSITION;
		                                     stmt_formula->name = strdup($1);
		                                     $$ = stmt_formula;
		                                   }

boolean: BOOLEAN_LITERAL                   {
		                                     Formula *stmt_formula = formula_new();
		                                     stmt_formula->type = CTL_TYPE_BOOLEAN_LITERAL;
		                                     gboolean value;

		                                     if (g_strcmp0($1, "true") == 0) {
		                                       value = TRUE;
		                                     } else {
		                                       value = FALSE;
		                                     }

		                                     stmt_formula->value = value;
		                                     $$ = stmt_formula;
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
