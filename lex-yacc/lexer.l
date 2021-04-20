dig [0-9]
let [A-Za-z]

%{
#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"
#include "ast.h"

int lineno = 1;
%}

%%
	/* KEYWORDS */
"if"                      return IF;
"else"                    return ELSE;
"while"                   return WHILE;
"var"                     return VAR;
"fun"                     return FUN;
"->"                      return STRELA;
"return"                  return RET;
({let}|_)({let}|{dig}|_)* { 
                              yylval.clan = new_ast_ident(lineno, yytext); 
                              return IDENT;
                          }
[0-9]+                    {
                              yylval.clan = new_ast_int(lineno, atol(yytext)); 
                              return LIT_INTEGER;
                          }
">="                      return GE;
"<="                      return LE;
"=="                      return EQ;
"!="                      return NE;
"++"                      return PP;
"--"                      return MM;
[-;+/*<>()=?:,]           return *yytext;
\n                        { lineno++; }
[ \t]+                    ; /* ignore whitespace */
.                         return *yytext; /* yyerror("greska"); */
%%

int yywrap() {
	return 1;
}
