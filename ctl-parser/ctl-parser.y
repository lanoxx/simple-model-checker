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
%token	<formula>		EXISTS
%token	<formula>		UNTIL
%token	<formula>		NOT

%type	<formula>		stmt
%type	<formula>		atom

%left   <formula>		AND
%nonassoc '('
%nonassoc ')'

%%

input:  %empty
		| stmt                             {
		                                     //printf("input is stmt\n");
		                                     root_formula = $1; YYACCEPT;
		                                   }
		;

stmt:   atom                               {
		                                     //printf("statement is atom\n");
		                                     $$ = $1;
		                                   }
		| NOT atom                         {
		                                     //printf("statement is negated statement\n");
		                                     Formula *stmt_formula = formula_new();
		                                     stmt_formula->type = CTL_TYPE_NEGATION;
		                                     stmt_formula->subformulas = g_list_append (NULL, $2);
		                                     stmt_formula->name = strdup("negation");
		                                     $$ = stmt_formula;
		                                   }
		| EXISTS_NEXT '(' stmt ')'         {
		                                     //printf("statement is EX\n");
		                                     Formula *stmt_formula = formula_new();
		                                     stmt_formula->type = CTL_TYPE_EXIST_NEXT;
		                                     stmt_formula->subformulas = g_list_append (NULL, $3);
		                                     stmt_formula->name = strdup("EX");
		                                     $$ = stmt_formula;
		                                   }
		| EXISTS_GLOBALLY '(' stmt ')'     {
		                                     //printf("statement is EG\n");
		                                     Formula *stmt_formula = formula_new();
		                                     stmt_formula->type = CTL_TYPE_EXISTS_GLOBALLY;
		                                     stmt_formula->subformulas = g_list_append (NULL, $3);
		                                     stmt_formula->name = strdup("EG");
		                                     $$ = stmt_formula;
		                                   }
		| EXISTS stmt UNTIL stmt           {
		                                     //printf("statement is E U\n");
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
		                                     //printf("statement is conjunction\n");
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
		                                     //printf("parsed atom\n");
		                                     Formula *stmt_formula = formula_new();
		                                     stmt_formula->type = CTL_TYPE_ATOMIC_PROPOSITION;
		                                     stmt_formula->name = strdup($1);
		                                     $$ = stmt_formula;
		                                   }

%%

int yyerror(const char *s)
{
  extern int yylineno;	// defined and maintained in lex.c
  extern char *yytext;	// defined and maintained in lex.c

  printf ("ERROR: %s at symbol \"%s", s, yytext);
  printf ("\" on line %d", yylineno);
  exit(1);
}
