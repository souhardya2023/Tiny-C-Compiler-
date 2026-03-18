%{
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"
%}

ws	        [ \t\n]+
ows         []
id	        [a-zA-Z_][a-zA-Z0-9_]*
num	        [0-9]+


int         int
float       float
double      double
long        long|long{ws}int
structure   struct
if          if
else        else
while       while


%%

{ws}	            {  }
{structure}         { yylval.str = strdup(yytext); return STRUCT; }
{int}	            { yylval.str = strdup(yytext); return INT; }
{float}	            { yylval.str = strdup(yytext); return FLT; }
{double}	        { yylval.str = strdup(yytext); return DBL; }
{long}	            { yylval.str = strdup(yytext); return LNG; }
{if}                {  return IF; }
{else}              { return ELSE; }
{while}             { return WHILE; }
{id}                {  yylval.str = strdup(yytext); return ID; }


\[	                { return yytext[0]; }
\]	                { return yytext[0]; }
\{                  { return yytext[0]; }
\}                  { return yytext[0]; }

{num}               { yylval.val = atoi(yytext); return INTC; }
{num}\.{num}?       { yylval.d = atof(yytext); return FLTC; }
\,	                { return yytext[0]; }
\;	                { return yytext[0]; }

\=                  { return yytext[0]; }

\+                  { return yytext[0]; }
\-                  { return yytext[0]; }
\/                  { return yytext[0]; }
\*	                { return yytext[0]; }
\(                  { return yytext[0]; }
\)                  { return yytext[0]; }
\%                  { return yytext[0]; }
\<                  { return yytext[0]; }
\>                  { return yytext[0]; }
\!                  { return yytext[0]; }
\&\&                { return AND; }
\|\|                { return OR; }
\.                  { return yytext[0]; }

.	                { fprintf(stderr, "*** Lex error: Bad character '%c'\n", yytext[0]); }

%%

int yywrap ( ) { return 1; }
