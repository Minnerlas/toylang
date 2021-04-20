#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"

const char *ast_ops[] = {
	[AST_DODELA]    = "=",

	[AST_PREDPP]    = "++{}",
	[AST_PREDMM]    = "--{}",
	[AST_POSLPP]    = "{}++",
	[AST_POSLMM]    = "{}--",

	[AST_PLUS]      = "+",
	[AST_MINUS]     = "-",
	[AST_UMINUS]    = "-1",
	[AST_PUTA]      = "*",
	[AST_UPUTA]     = "*1",
	[AST_PODELJENO] = "/",
	[AST_MOD] = "%",

	[AST_VECE]      = ">",
	[AST_MANJE]     = "<",
	[AST_VECEJED]   = ">=",
	[AST_MANJEJED]  = "<=",
	[AST_JEDNAKO]   = "==",
	[AST_NEJED]     = "!=",
	[AST_TRINARNI]  = "?:",

	[AST_FUNCALL]   = "funcall",
};

const char *stmt_tip[] = {
	[STMT_NULL]   = "NULL",
	[STMT_BLOK]   = "BLOK",
	[STMT_OP]     = "expr",
	[STMT_IF]     = "IF",
	[STMT_WHILE]  = "WHILE",
	[STMT_VARDEC] = "VARDEC",
};


struct ast_clan *new_ast_int(int lineno, long a) {
	struct ast_clan *tmp = calloc(1, sizeof(*tmp));
	tmp->lineno = lineno;
	tmp->tip = CLAN_INT;
	tmp->clan_int.vred = a;
	return tmp;
}

void free_ast_int(struct ast_clan *clan) {
	free(clan);
}

struct ast_clan *new_ast_ident(int lineno, const char *a) {
	struct ast_clan *tmp = calloc(1, sizeof(*tmp));
	tmp->lineno = lineno;
	tmp->tip = CLAN_IDENT;
	tmp->clan_ident.ime = strdup(a);
	return tmp;
}

void free_ast_ident(struct ast_clan *clan) {
	free(clan->clan_ident.ime);
	free(clan);
}

struct ast_clan *new_ast_op(int lineno, enum ast_op op, struct ast_clan *p1, 
		struct ast_clan *p2, struct ast_clan *p3) {
	struct ast_clan *tmp = calloc(1, sizeof(*tmp));
	tmp->lineno = lineno;
	tmp->tip = CLAN_OP;
	tmp->clan_op.op = op;
	tmp->clan_op.deca[0] = p1;
	tmp->clan_op.deca[1] = p2;
	tmp->clan_op.deca[2] = p3;
	return tmp;
}


void free_ast_op(struct ast_clan *clan) {
	for (int i = 0; i < 3; i++)
		if (clan->clan_op.deca[i])
			free_ast_stablo(clan->clan_op.deca[i]);
	free(clan);
}

struct ast_clan *new_ast_stmt_null(int lineno) {
	struct ast_clan *tmp = calloc(1, sizeof(*tmp));
	tmp->lineno = lineno;
	tmp->tip = CLAN_STMT;
	tmp->clan_stmt.tip = STMT_NULL;

	return tmp;
}

struct ast_clan *new_ast_stmt_blok(int lineno, struct ast_clan *blok) {
	struct ast_clan *tmp = calloc(1, sizeof(*tmp));
	tmp->lineno = lineno;
	tmp->tip = CLAN_STMT;
	tmp->clan_stmt.tip = STMT_BLOK;
	tmp->clan_stmt.stmt_blok = blok;

	return tmp;
}

struct ast_clan *new_ast_stmt_expr(int lineno, struct ast_clan *expr) {
	struct ast_clan *tmp = calloc(1, sizeof(*tmp));
	tmp->lineno = lineno;
	tmp->tip = CLAN_STMT;
	tmp->clan_stmt.tip = STMT_OP;
	tmp->clan_stmt.op = expr;

	return tmp;
}

struct ast_clan *new_ast_stmt_if(int lineno, struct ast_clan *usl,
	struct ast_clan *blok, struct ast_clan *inace) {

	struct ast_clan *tmp = calloc(1, sizeof(*tmp));
	tmp->lineno = lineno;
	tmp->tip = CLAN_STMT;
	tmp->clan_stmt.tip = STMT_IF;
	tmp->clan_stmt.if_stmt.usl   = usl;
	tmp->clan_stmt.if_stmt.blok  = blok;
	tmp->clan_stmt.if_stmt.inace = inace;

	return tmp;
}

struct ast_clan *new_ast_stmt_while(int lineno, struct ast_clan *usl,
	struct ast_clan *blok) {

	struct ast_clan *tmp = calloc(1, sizeof(*tmp));
	tmp->lineno = lineno;
	tmp->tip = CLAN_STMT;
	tmp->clan_stmt.tip = STMT_WHILE;
	tmp->clan_stmt.while_stmt.usl   = usl;
	tmp->clan_stmt.while_stmt.blok  = blok;

	return tmp;
}


struct ast_clan *new_ast_stmt_vardec(int lineno, struct ast_clan *vardec) {
	struct ast_clan *tmp = calloc(1, sizeof(*tmp));
	tmp->lineno = lineno;
	tmp->tip = CLAN_STMT;
	tmp->clan_stmt.tip = STMT_VARDEC;
	tmp->clan_stmt.vardec = vardec;

	return tmp;
}

struct ast_clan *new_ast_stmt_ret(int lineno, struct ast_clan *expr) {
	struct ast_clan *tmp = calloc(1, sizeof(*tmp));
	tmp->lineno = lineno;
	tmp->tip = CLAN_STMT;
	tmp->clan_stmt.tip = STMT_RET;
	tmp->clan_stmt.ret = expr;

	return tmp;
}

void free_ast_stmt(struct ast_clan *clan) {
	switch(clan->clan_stmt.tip) {
		case STMT_NULL:
			break;
		case STMT_BLOK:
			free_ast_stablo(clan->clan_stmt.stmt_blok);
			break;
		case STMT_OP:
			free_ast_stablo(clan->clan_stmt.op);
			break;
		case STMT_IF:
			free_ast_op(clan->clan_stmt.if_stmt.usl);
			free_ast_stablo(clan->clan_stmt.if_stmt.blok);
			if (clan->clan_stmt.if_stmt.inace)
				free_ast_stablo(clan->clan_stmt.if_stmt.inace);
			break;
		case STMT_WHILE:
			free_ast_op(clan->clan_stmt.while_stmt.usl);
			free_ast_stablo(clan->clan_stmt.while_stmt.blok);
			break;
		case STMT_VARDEC:
			free_ast_vardec(clan->clan_stmt.vardec);
			break;
		case STMT_RET:
			free_ast_stablo(clan->clan_stmt.ret);
			break;
/* TODO */
		default:
			fprintf(stderr, "Ovo je greška!!!\n");
			return;
	}
	free(clan);
}

struct ast_clan *new_ast_stmtlist(int lineno, struct ast_clan *stmt) {
	struct ast_clan *tmp = calloc(1, sizeof(*tmp));
	tmp->lineno = lineno;
	tmp->tip = CLAN_STMTLIST;
	tmp->clan_stmtlist.dniz = new_dniz();
	add_dniz(tmp->clan_stmtlist.dniz, stmt);


	return tmp;
}

struct ast_clan *append_ast_stmtlist(int lineno, struct ast_clan *list, 
		struct ast_clan *stmt) {

	(void)lineno;
	add_dniz(list->clan_stmtlist.dniz, stmt);

	return list;
}

void free_ast_stmtlist(struct ast_clan *lista) {
	for (size_t i = 0; i < lista->clan_stmtlist.dniz->vel; i++)
		free_ast_stablo(lista->clan_stmtlist.dniz->niz[i]);

	free_dniz(lista->clan_stmtlist.dniz);
	free(lista);
}

struct ast_clan *new_ast_blok(int lineno, struct ast_clan *stmt_list) {
	struct ast_clan *tmp = calloc(1, sizeof(*tmp));
	tmp->lineno = lineno;
	tmp->tip = CLAN_BLOK;

	tmp->clan_blok.stmt_list = stmt_list;

	return tmp;
}

void free_ast_blok(struct ast_clan *blok) {
	free_ast_stmtlist(blok->clan_blok.stmt_list);
	free(blok);
}

struct ast_clan *new_ast_vardec(int lineno, struct ast_clan *ime,
	struct ast_clan *tip, struct ast_clan *expr) {

	struct ast_clan *tmp = calloc(1, sizeof(*tmp));
	tmp->lineno = lineno;
	tmp->tip = CLAN_VARDEC;
	tmp->clan_vardec.niz = new_dniz();

	struct var_dec *t = malloc(sizeof(*t));
	t->ime  = ime;
	t->tip  = tip;
	t->expr = expr;
	add_dniz(tmp->clan_vardec.niz, t);

	return tmp;
}

struct ast_clan *add_ast_vardec(struct ast_clan *vardec, struct ast_clan *ime,
	struct ast_clan *tip, struct ast_clan *expr) {

	struct var_dec *t = malloc(sizeof(*t));
	t->ime  = ime;
	t->tip  = tip;
	t->expr = expr;
	add_dniz(vardec->clan_vardec.niz, t);

	return vardec;
}

void free_ast_vardec(struct ast_clan *vardec) {
	for (size_t i = 0; i < vardec->clan_vardec.niz->vel; i++) {
		struct var_dec *t = vardec->clan_vardec.niz->niz[i];
		free_ast_stablo(t->ime);
		free_ast_stablo(t->tip);
		if (t->expr)
			free_ast_stablo(t->expr);
		free(t);
	}
	free_dniz(vardec->clan_vardec.niz);
	free(vardec);
}

struct ast_clan *new_ast_args(int lineno, struct ast_clan *ime,
	struct ast_clan *tip) {

	struct ast_clan *tmp = calloc(1, sizeof(*tmp));
	tmp->lineno = lineno;
	tmp->tip = CLAN_ARGS;
	tmp->clan_args.niz = new_dniz();

	struct ast_arg *t = malloc(sizeof(*t));
	t->ime  = ime;
	t->tip  = tip;
	add_dniz(tmp->clan_args.niz, t);

	return tmp;
}

struct ast_clan *add_ast_args(struct ast_clan *vardec, struct ast_clan *ime,
	struct ast_clan *tip) {

	if (!vardec) {
		vardec = new_ast_args(ime->lineno, ime, tip);
		return vardec;
	}
	struct ast_arg *t = malloc(sizeof(*t));
	t->ime  = ime;
	t->tip  = tip;
	add_dniz(vardec->clan_args.niz, t);

	return vardec;
}

void free_ast_args(struct ast_clan *vardec) {
	for (size_t i = 0; i < vardec->clan_args.niz->vel; i++) {
		struct ast_arg *t = vardec->clan_args.niz->niz[i];
		free_ast_stablo(t->ime);
		free_ast_stablo(t->tip);
		free(t);
	}
	free_dniz(vardec->clan_args.niz);
	free(vardec);
}

struct ast_clan *new_ast_fundef(int lineno, struct ast_clan *ime,
	struct ast_clan *args, struct ast_clan *ret, struct ast_clan *blok) {

	struct ast_clan *tmp = calloc(1, sizeof(*tmp));
	tmp->lineno = lineno;
	tmp->tip = CLAN_FUNDEF;

	tmp->clan_fundef.ime  = ime;
	tmp->clan_fundef.args = args;
	tmp->clan_fundef.ret  = ret;
	tmp->clan_fundef.blok = blok;

	return tmp;
}

void free_ast_fundef(struct ast_clan *fundef) {
	free_ast_stablo(fundef->clan_fundef.ime);
	if (fundef->clan_fundef.args)
		free_ast_stablo(fundef->clan_fundef.args);
	if (fundef->clan_fundef.ret)
		free_ast_stablo(fundef->clan_fundef.ret);
	free_ast_stablo(fundef->clan_fundef.blok);
	free(fundef);
}

struct ast_clan *new_ast_paramlist(int lineno, struct ast_clan *expr) {
	struct ast_clan *tmp = calloc(1, sizeof(*tmp));
	tmp->lineno = lineno;
	tmp->tip = CLAN_PARAMLIST;

	tmp->clan_paramlist.niz = new_dniz();
	add_dniz(tmp->clan_paramlist.niz, expr);

	return tmp;
}

struct ast_clan *add_ast_paramlist(struct ast_clan *params,
	struct ast_clan *expr) {

	add_dniz(params->clan_paramlist.niz, expr);
	return params;
}

void free_ast_paramlist(struct ast_clan *params) {
	for (size_t i = 0; i < params->clan_paramlist.niz->vel; i++)
		free_ast_stablo(params->clan_paramlist.niz->niz[i]);
	free_dniz(params->clan_paramlist.niz);
	free(params);
}

static void uvuci(int n) {
	for (int i = 0; i < n; i++)
		// printf("  ");
		putchar('\t');
}
static void ast_print_int(int u, struct ast_clan *clan) {
	uvuci(u);
	printf("INT:%d %ld\n", clan->lineno, clan->clan_int.vred);

}

static void ast_print_ident(int u, struct ast_clan *clan) {
	uvuci(u);
	printf("IDENT:%d %s\n", clan->lineno, clan->clan_ident.ime);
}

static void ast_print_op(int u, struct ast_clan *clan) {
	uvuci(u);
	printf("OP:%d %s\n", clan->lineno, ast_ops[clan->clan_op.op]);
	for (int i = 0; i < 3; i++)
		if (clan->clan_op.deca[i])
			ast_print_stablo(u+1, clan->clan_op.deca[i]);
}

static void ast_print_stmt(int u, struct ast_clan *clan) {
	uvuci(u);
	printf("STMT:%d (tip %s)\n", clan->lineno, stmt_tip[clan->clan_stmt.tip]);
	switch(clan->clan_stmt.tip) {
		case STMT_NULL:
			break;
		case STMT_BLOK:
			ast_print_blok(u+1, clan->clan_stmt.stmt_blok);
			break;
		case STMT_OP:
			ast_print_stablo(u+1, clan->clan_stmt.op);
			break;
		case STMT_IF:
			uvuci(u); printf("IF\n");
			ast_print_stablo(u+1, clan->clan_stmt.if_stmt.usl);
			uvuci(u); printf("THEN\n");
			ast_print_stablo(u+1, clan->clan_stmt.if_stmt.blok);
			if (clan->clan_stmt.if_stmt.inace) {
				uvuci(u); printf("ELSE\n");
				ast_print_stablo(u+1, clan->clan_stmt.if_stmt.inace);
			}
			break;
		case STMT_WHILE:
			uvuci(u); printf("WHILE\n");
			ast_print_stablo(u+1, clan->clan_stmt.if_stmt.usl);
			uvuci(u); printf("DO\n");
			ast_print_stablo(u+1, clan->clan_stmt.if_stmt.blok);
			break;
		case STMT_VARDEC:
			ast_print_vardec(u+1, clan->clan_stmt.vardec);
			break;
		case STMT_RET:
			uvuci(u); printf("RET\n");
			ast_print_stablo(u+1, clan->clan_stmt.ret);
			break;
		default:
			fprintf(stderr, "Ovo je greška!!!!\n");
	}
}

static void ast_print_stmtlist(int u, struct ast_clan *lista) {
	uvuci(u);
	printf("STMT_LIST:%d \n", lista->lineno);
	for (size_t i = 0; i < lista->clan_stmtlist.dniz->vel; i++)
		ast_print_stablo(u+1, lista->clan_stmtlist.dniz->niz[i]);
}

void ast_print_blok(int u, struct ast_clan *blok) {
	uvuci(u);
	printf("BLOK:%d\n", blok->lineno);
	if (blok->clan_blok.stmt_list)
		ast_print_stmtlist(u+1, blok->clan_blok.stmt_list);
	else {
		uvuci(u+1);
		printf("(null)\n");
	}
}

void ast_print_vardec(int u, struct ast_clan *vardec) {
	uvuci(u);
	printf("VARDEC:%d\n", vardec->lineno);
	for (size_t i = 0; i < vardec->clan_vardec.niz->vel; i++) {
		struct var_dec *t = vardec->clan_vardec.niz->niz[i];
		uvuci(u+1);
		printf("IDENT_IME(%s): IDENT_TIP(%s)\n", t->ime->clan_ident.ime,
			t->tip->clan_ident.ime);
		if (t->expr) {
			uvuci(u+1);
			printf("=\n");
			ast_print_stablo(u+2, t->expr);
		}
	}
}

void ast_print_args(int u, struct ast_clan *vardec) {
	uvuci(u);
	printf("ARGS:%d\n", vardec->lineno);
	for (size_t i = 0; i < vardec->clan_args.niz->vel; i++) {
		struct var_dec *t = vardec->clan_args.niz->niz[i];
		uvuci(u+1);
		printf("IDENT_IME(%s): IDENT_TIP(%s)\n", t->ime->clan_ident.ime,
			t->tip->clan_ident.ime); // Verovatno će biti izmenjeno
	}
}

void ast_print_fundef(int u, struct ast_clan *fundef) {
	uvuci(u);
	printf("FUNDEF:% 4d %s (", fundef->lineno,
		fundef->clan_fundef.ime->clan_ident.ime);
	if (fundef->clan_fundef.args) {
		putchar('\n');
		ast_print_args(u+1, fundef->clan_fundef.args);
		uvuci(u);
		printf(")");
	} else
		printf(")");

	if (fundef->clan_fundef.ret) {
		printf(" -> ");
		ast_print_ident(0, fundef->clan_fundef.ret);
	} else
		putchar('\n');

	ast_print_blok(u+1, fundef->clan_fundef.blok);
}

void ast_print_paramlist(int u, struct ast_clan *stablo) {
	uvuci(u);
	printf("PARAMS:%d\n", stablo->lineno);

	for (size_t i = 0; i < stablo->clan_paramlist.niz->vel; i++)
		ast_print_stablo(u+1, stablo->clan_paramlist.niz->niz[i]);

}

void free_ast_stablo(struct ast_clan *stablo) {
	if (!stablo) {
		fprintf(stderr, "\x1b[31m(free) Stablo je prazno\x1b[0m\n");
		return;
	}
	switch(stablo->tip) {
		case CLAN_INT:
			free_ast_int(stablo);
			break;
		case CLAN_IDENT:
			free_ast_ident(stablo);
			break;
		case CLAN_OP:
			free_ast_op(stablo);
			break;
		case CLAN_STMT:
			free_ast_stmt(stablo);
			break;
		case CLAN_STMTLIST:
			free_ast_stmtlist(stablo);
			break;
		case CLAN_BLOK:
			free_ast_blok(stablo);
			break;
		case CLAN_VARDEC:
			free_ast_vardec(stablo);
			break;
		case CLAN_ARGS:
			free_ast_args(stablo);
			break;
		case CLAN_FUNDEF:
			free_ast_fundef(stablo);
			break;
		case CLAN_PARAMLIST:
			free_ast_paramlist(stablo);
			break;
		default:
			fprintf(stderr, "Ovo je greška! %d\n", stablo->tip);
	}
}

void ast_print_stablo(int u, struct ast_clan *stablo) {
	if (!stablo) {
		fprintf(stderr, "\x1b[31m(print) Stablo je prazno\x1b[0m\n");
		return;
	}
	switch(stablo->tip) {
		case CLAN_INT:
			ast_print_int(u, stablo);
			break;
		case CLAN_IDENT:
			ast_print_ident(u, stablo);
			break;
		case CLAN_OP:
			ast_print_op(u, stablo);
			break;
		case CLAN_STMT:
			ast_print_stmt(u, stablo);
			break;
		case CLAN_STMTLIST:
			ast_print_stmtlist(u, stablo);
			break;
		case CLAN_BLOK:
			ast_print_blok(u, stablo);
			break;
		case CLAN_VARDEC:
			ast_print_vardec(u, stablo);
			break;
		case CLAN_ARGS:
			ast_print_args(u, stablo);
			break;
		case CLAN_FUNDEF:
			ast_print_fundef(u, stablo);
			break;
		case CLAN_PARAMLIST:
			ast_print_paramlist(u, stablo);
			break;
		default:
			fprintf(stderr, "Ovo je greška!!\n");
			if (stablo)
				fprintf(stderr, "%d\n", stablo->tip);
	}
}
