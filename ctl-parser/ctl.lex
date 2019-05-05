/* Mini Calculator */
/* calc.lex */

%{
#include "heading.h"
#include <bison.h>
int yyerror(char *s);

#undef YY_INPUT
#define YY_INPUT(buffer,result,maxsize) \
{ \
  int remaining = strlen(global_lexer_input); \
  int length = remaining > maxsize ? maxsize : remaining; \
  strncpy (buffer, global_lexer_input, length); \
  global_lexer_input += length; \
  result = length; \
}
%}

atom		[a-z]

negation	NOT|not
and			AND|and

exists_next		EX
exists_globally	EG
exists			E
until			U

opening_parenthesis		\(
closing_parenthesis		\)

%%

{exists_next}			{ char*token = malloc(strlen(yytext)+1); strcpy(token,yytext); yylval.atom_name = token; return EXISTS_NEXT; }
{exists_globally}		{ char*token = malloc(strlen(yytext)+1); strcpy(token,yytext); yylval.atom_name = token; return EXISTS_GLOBALLY; }
{exists}				{ char*token = malloc(strlen(yytext)+1); strcpy(token,yytext); yylval.atom_name = token; return EXISTS; }
{until}					{ char*token = malloc(strlen(yytext)+1); strcpy(token,yytext); yylval.atom_name = token; return UNTIL; }
{negation}				{ char*token = malloc(strlen(yytext)+1); strcpy(token,yytext); yylval.atom_name = token; return NOT; }
{and}					{ char*token = malloc(strlen(yytext)+1); strcpy(token,yytext); yylval.atom_name = token; return AND; }
{atom}					{ char*token = malloc(strlen(yytext)+1); strcpy(token,yytext); yylval.atom_name = token; return ATOMIC_PROPOSITION; }
{opening_parenthesis}	{ return '('; }
{closing_parenthesis}	{ return ')'; }


[ \t]*		{ }
[\n]		{ yylineno++;	}

.		{ printf("%s", "LEXER "); yyerror("XXX"); exit(1);	}

