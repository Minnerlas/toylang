#ifndef AST_H
#define AST_H

#include "util.h"

struct ast_clan;

struct ast_clan_int {
	long vred;
};

struct ast_clan_expr {
	int ndece;
	struct ast_clan *deca[3];
};

struct ast_clan_ident {
	char *ime;
};

enum ast_op {
	AST_DODELA,

	AST_PREDPP,
	AST_PREDMM,
	AST_POSLPP,
	AST_POSLMM,

	AST_PLUS,
	AST_MINUS,
	AST_UMINUS,
	AST_PUTA,
	AST_UPUTA,
	AST_PODELJENO,
	AST_MOD,
	AST_UZV,

	AST_VECE,
	AST_MANJE,
	AST_VECEJED,
	AST_MANJEJED,
	AST_JEDNAKO,
	AST_NEJED,
	AST_TRINARNI,

	AST_FUNCALL,
};

struct ast_clan_op {
	enum ast_op op;
	struct ast_clan *deca[3];
};

enum ast_stmt_tip {
	STMT_NULL,
	STMT_BLOK,
	STMT_OP,
	STMT_IF,
	STMT_WHILE,
	STMT_VARDEC,
	STMT_RET,
};

struct ast_clan_stmt {
	enum ast_stmt_tip tip;
	union {
		struct ast_clan *op;
		struct ast_clan *stmt_blok;
		struct {
			struct ast_clan *usl;
			struct ast_clan *blok;
			struct ast_clan *inace;
		} if_stmt;
		struct {
			struct ast_clan *usl;
			struct ast_clan *blok;
		} while_stmt;
		struct ast_clan *vardec;
		struct ast_clan *ret;
	};
};

enum ast_clan_tip {
	CLAN_NISTA,
	CLAN_INT,
	CLAN_EXPR,
	CLAN_IDENT,
	CLAN_OP,
	CLAN_STMT,
	CLAN_STMTLIST,
	CLAN_BLOK,
	CLAN_VARDEC,
	CLAN_ARGS,
	CLAN_FUNDEF,
	CLAN_PARAMLIST,
};

struct ast_clan_stmtlist {
	struct dniz *dniz;
};

struct ast_clan_blok {
	struct ast_clan *stmt_list;
};

struct var_dec {
	struct ast_clan *ime;
	struct ast_clan *tip;
	struct ast_clan *expr;
};

struct ast_clan_vardec {
	struct dniz *niz;
};

struct ast_arg {
	struct ast_clan *ime;
	struct ast_clan *tip;
};

struct ast_clan_args {
	struct dniz *niz;
};
// typedef struct dniz *ast_clan_args;

struct ast_clan_fundef {
	struct ast_clan *ime;
	struct ast_clan *args;
	struct ast_clan *ret;
	struct ast_clan *blok;
};

struct ast_clan_paramlist {
	struct dniz *niz;
};

struct ast_clan_funcall {
	struct ast_clan *ident;
	struct ast_clan *params;
};

union ast_clanovi {
	struct ast_clan_int clan_int;
	struct ast_clan_ident clan_ident;
	struct ast_clan_op clan_op;
	struct ast_clan_stmt clan_stmt;
	struct ast_clan_stmtlist clan_stmtlist;
	struct ast_clan_blok clan_blok;
	struct ast_clan_vardec clan_vardec;
	struct ast_clan_args clan_args;
	struct ast_clan_fundef clan_fundef;
	struct ast_clan_paramlist clan_paramlist;
	struct ast_clan_funcall clan_funcall;
};

struct ast_clan {
	int lineno;
	enum ast_clan_tip tip;
	union {
		struct ast_clan_int clan_int;
		struct ast_clan_ident clan_ident;
		struct ast_clan_op clan_op;
		struct ast_clan_stmt clan_stmt;
		struct ast_clan_stmtlist clan_stmtlist;
		struct ast_clan_blok clan_blok;
		struct ast_clan_vardec clan_vardec;
		struct ast_clan_args clan_args;
		struct ast_clan_fundef clan_fundef;
		struct ast_clan_paramlist clan_paramlist;
		struct ast_clan_funcall clan_funcall;
	};
};

/* Funkcije */

struct ast_clan *new_ast_int(int lineno, long a);
struct ast_clan *new_ast_ident(int lineno, const char *a);
struct ast_clan *new_ast_op(int lineno, enum ast_op op, struct ast_clan *p1, 
		struct ast_clan *p2, struct ast_clan *p3);

struct ast_clan *new_ast_stmt_null(int lineno);
struct ast_clan *new_ast_stmt_blok(int lineno, struct ast_clan *blok);
struct ast_clan *new_ast_stmt_expr(int lineno, struct ast_clan *expr);
struct ast_clan *new_ast_stmt_if(int lineno, struct ast_clan *usl,
	struct ast_clan *blok, struct ast_clan *inace);
struct ast_clan *new_ast_stmt_while(int lineno, struct ast_clan *usl,
	struct ast_clan *blok);
struct ast_clan *new_ast_stmt_vardec(int lineno, struct ast_clan *vardec);
struct ast_clan *new_ast_stmt_ret(int lineno, struct ast_clan *expr);

struct ast_clan *new_ast_stmtlist(int lineno, struct ast_clan *stmt);
struct ast_clan *append_ast_stmtlist(int lineno, struct ast_clan *list,
	struct ast_clan *stmt);

struct ast_clan *new_ast_blok(int lineno, struct ast_clan *stmt_list);

struct ast_clan *new_ast_vardec(int lineno, struct ast_clan *ime,
	struct ast_clan *tip, struct ast_clan *expr);
struct ast_clan *add_ast_vardec(struct ast_clan *vardec, struct ast_clan *ime,
	struct ast_clan *tip, struct ast_clan *expr);

struct ast_clan *new_ast_args(int lineno, struct ast_clan *ime,
	struct ast_clan *tip);
struct ast_clan *add_ast_args(struct ast_clan *vardec, struct ast_clan *ime,
	struct ast_clan *tip);

struct ast_clan *new_ast_fundef(int lineno, struct ast_clan *ime,
	struct ast_clan *args, struct ast_clan *ret, struct ast_clan *blok);

struct ast_clan *new_ast_paramlist(int lineno, struct ast_clan *expr);
struct ast_clan *add_ast_paramlist(struct ast_clan *params,
	struct ast_clan *expr);

void free_ast_stablo(struct ast_clan *stablo);
void free_ast_vardec(struct ast_clan *vardec);
void ast_print_stablo(int u, struct ast_clan *stablo);

void ast_print_blok(int u, struct ast_clan *blok);
void ast_print_vardec(int u, struct ast_clan *vardec);
#endif /* AST_H */
