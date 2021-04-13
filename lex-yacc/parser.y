%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "ast.h"

int yylex();
void yyerror(char *s);
extern int lineno;
%}

%union {
	// int iValue;
	struct ast_clan *clan;
}

%token <clan> LIT_INTEGER IDENT
%token IF WHILE VAR
%nonassoc IFX
%nonassoc ELSE

%type  <clan> expr stmt stmt_list blok var_list

%right '='
%right '?' ':'
%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc UMINUS UPUTA PP MM
%nonassoc POSTF

%%
prg: stmt_list { ast_print_stablo(0, $1); free_ast_stablo($1); }
   ;

blok: '{' stmt_list '}'		{ $$ = new_ast_blok(lineno, $2); }
	| '{' '}'				{ $$ = new_ast_blok(lineno, NULL); }
	;

stmt_list: stmt				{ $$ = new_ast_stmtlist(lineno, $1); }
		 | stmt_list stmt	{ $$ = append_ast_stmtlist(lineno, $1, $2); }
		 ;

var_list: IDENT ':' IDENT '=' expr				{ $$ = new_ast_vardec(lineno, $1, $3, $5); }
		| IDENT ':' IDENT 						{ $$ = new_ast_vardec(lineno, $1, $3, NULL); }
		| var_list ',' IDENT ':' IDENT			{ $$ = add_ast_vardec($1, $3, $5, NULL); }
		| var_list ',' IDENT ':' IDENT '=' expr	{ $$ = add_ast_vardec($1, $3, $5, $7); printf("%p\n", $7); }
		;

stmt: ';'								{ $$ = new_ast_stmt_null(lineno); }
	| expr ';'  						{ $$ = new_ast_stmt_expr(lineno, $1); }
	| blok								{ $$ = new_ast_stmt_blok(lineno, $1); }
	| IF '(' expr ')' stmt %prec IFX	{ $$ = new_ast_stmt_if(lineno, $3, $5, NULL); }
	| IF '(' expr ')' stmt ELSE stmt	{ $$ = new_ast_stmt_if(lineno, $3, $5, $7); }
	| WHILE '(' expr ')' stmt 			{ $$ = new_ast_stmt_while(lineno, $3, $5); }
	| VAR var_list ';'					{ $$ = new_ast_stmt_vardec(lineno, $2); }
	;

expr: LIT_INTEGER				{ $$ = $1; }
	| IDENT						{ $$ = $1; }
	| PP expr					{ $$ = new_ast_op(lineno, AST_PREDPP,    $2, NULL, NULL); }
	| MM expr					{ $$ = new_ast_op(lineno, AST_PREDMM,    $2, NULL, NULL); }
	| expr PP  %prec POSTF		{ $$ = new_ast_op(lineno, AST_POSLPP,    $1, NULL, NULL); }
	| expr MM  %prec POSTF		{ $$ = new_ast_op(lineno, AST_POSLMM,    $1, NULL, NULL); }
	| '*' expr %prec UPUTA		{ $$ = new_ast_op(lineno, AST_UPUTA,     $2, NULL, NULL); }
	| '-' expr %prec UMINUS		{ $$ = new_ast_op(lineno, AST_UMINUS,    $2, NULL, NULL); }
	| expr '=' expr 			{ $$ = new_ast_op(lineno, AST_DODELA,    $1, $3,   NULL); }
	| expr '+' expr 			{ $$ = new_ast_op(lineno, AST_PLUS,      $1, $3,   NULL); }
	| expr '-' expr 			{ $$ = new_ast_op(lineno, AST_MINUS,     $1, $3,   NULL); }
	| expr '*' expr 			{ $$ = new_ast_op(lineno, AST_PUTA,      $1, $3,   NULL); }
	| expr '/' expr 			{ $$ = new_ast_op(lineno, AST_PODELJENO, $1, $3,   NULL); }
	| expr '%' expr 			{ $$ = new_ast_op(lineno, AST_MOD,		 $1, $3,   NULL); }
	| expr '<' expr 			{ $$ = new_ast_op(lineno, AST_MANJE,     $1, $3,   NULL); }
	| expr '>' expr 			{ $$ = new_ast_op(lineno, AST_VECE,      $1, $3,   NULL); }
	| expr GE  expr 			{ $$ = new_ast_op(lineno, AST_VECEJED,   $1, $3,   NULL); }
	| expr LE  expr 			{ $$ = new_ast_op(lineno, AST_MANJEJED,  $1, $3,   NULL); }
	| expr EQ  expr 			{ $$ = new_ast_op(lineno, AST_JEDNAKO,   $1, $3,   NULL); }
	| expr NE  expr 			{ $$ = new_ast_op(lineno, AST_NEJED,     $1, $3,   NULL); }
	| expr '?' expr ':' expr	{ $$ = new_ast_op(lineno, AST_TRINARNI,  $1, $3,   $5); }
	| '(' expr ')'				{ $$ = $2; }
	;

/*
arg_list: expr              {}
		| arg_list ',' expr {}
		| // NULL
		;
*/
%%

/* TODO: Napraviti da radi bolje */
void yyerror(char *s) {
	fprintf(stderr, "[Error: %d] %s in %c\n", lineno, s, yychar);
}

int main() {
	yyparse();
}
