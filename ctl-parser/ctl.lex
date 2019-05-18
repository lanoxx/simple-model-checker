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

{boolean}				{ yylval.atom_name = yytext; return BOOLEAN_LITERAL; }
{exists_next}			{ yylval.atom_name = yytext; return EXISTS_NEXT; }
{exists_globally}		{ yylval.atom_name = yytext; return EXISTS_GLOBALLY; }
{exists_future}			{ yylval.atom_name = yytext; return EXISTS_FUTURE; }
{always_next}			{ yylval.atom_name = yytext; return ALWAYS_NEXT; }
{always_globally}		{ yylval.atom_name = yytext; return ALWAYS_GLOBALLY; }
{always_future}			{ yylval.atom_name = yytext; return ALWAYS_FUTURE; }
{exists}				{ yylval.atom_name = yytext; return EXISTS; }
{always}				{ yylval.atom_name = yytext; return ALWAYS; }
{until}					{ yylval.atom_name = yytext; return UNTIL; }
{negation}				{ yylval.atom_name = yytext; return NOT; }
{and}					{ yylval.atom_name = yytext; return AND; }
{atom}					{ yylval.atom_name = yytext; return ATOMIC_PROPOSITION; }
{opening_parenthesis}	{ return '('; }
{closing_parenthesis}	{ return ')'; }


[ \t]*		{ }
[\n]		{ yylineno++;	}

.		{ printf("%s", "LEXER "); yyerror("XXX"); exit(1);	}

