/* Mini Calculator */
/* calc.lex */

%{
#include <ctl-parser/heading.h>
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

boolean		true|TRUE|false|FALSE

atom		[a-z]+

negation	NOT|not
and			AND|and

exists_next		EX
exists_globally	EG
exists_future	EF
always_next		AX
always_globally	AG
always_future	AF
exists			E
always			A
until			U

opening_parenthesis		\(
closing_parenthesis		\)

%%

{boolean}				{ char*token = malloc(strlen(yytext)+1); strcpy(token,yytext); yylval.atom_name = token; return BOOLEAN_LITERAL; }
{exists_next}			{ char*token = malloc(strlen(yytext)+1); strcpy(token,yytext); yylval.atom_name = token; return EXISTS_NEXT; }
{exists_globally}		{ char*token = malloc(strlen(yytext)+1); strcpy(token,yytext); yylval.atom_name = token; return EXISTS_GLOBALLY; }
{exists_future}			{ char*token = malloc(strlen(yytext)+1); strcpy(token,yytext); yylval.atom_name = token; return EXISTS_FUTURE; }
{always_next}			{ char*token = malloc(strlen(yytext)+1); strcpy(token,yytext); yylval.atom_name = token; return ALWAYS_NEXT; }
{always_globally}		{ char*token = malloc(strlen(yytext)+1); strcpy(token,yytext); yylval.atom_name = token; return ALWAYS_GLOBALLY; }
{always_future}			{ char*token = malloc(strlen(yytext)+1); strcpy(token,yytext); yylval.atom_name = token; return ALWAYS_FUTURE; }
{exists}				{ char*token = malloc(strlen(yytext)+1); strcpy(token,yytext); yylval.atom_name = token; return EXISTS; }
{always}				{ char*token = malloc(strlen(yytext)+1); strcpy(token,yytext); yylval.atom_name = token; return ALWAYS; }
{until}					{ char*token = malloc(strlen(yytext)+1); strcpy(token,yytext); yylval.atom_name = token; return UNTIL; }
{negation}				{ char*token = malloc(strlen(yytext)+1); strcpy(token,yytext); yylval.atom_name = token; return NOT; }
{and}					{ char*token = malloc(strlen(yytext)+1); strcpy(token,yytext); yylval.atom_name = token; return AND; }
{atom}					{ char*token = malloc(strlen(yytext)+1); strcpy(token,yytext); yylval.atom_name = token; return ATOMIC_PROPOSITION; }
{opening_parenthesis}	{ return '('; }
{closing_parenthesis}	{ return ')'; }


[ \t]*		{ }
[\n]		{ yylineno++;	}

.		{ printf("%s", "LEXER "); yyerror("XXX"); exit(1);	}

