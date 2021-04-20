#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

struct dniz {
	size_t vel;
	size_t stvvel;
	void **niz;
};

struct hmapa {
	struct dniz niz;
};

struct hclan {
	char *ime;
	uint64_t hesv;
	void* pok;
};

/* Funckije */

struct dniz *new_dniz();
void free_dniz(struct dniz *dniz);
int add_dniz(struct dniz *dniz, void *ul);
void clr_dniz(struct dniz *dniz);


struct hmapa *new_hmapa();
void free_hmapa(struct hmapa *mapa);
struct hmapa *add_hmapa(struct hmapa *mapa, const char *ime, void *pod);
struct hclan* find_hmapa(struct hmapa* h, const char* ime);
void hstampaj(struct hmapa *mapa);
#endif /* UTIL_H */
