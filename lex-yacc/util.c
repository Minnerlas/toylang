#include "util.h"
#include <stdio.h>

struct dniz *new_dniz() {
	struct dniz *tmp = malloc(sizeof(*tmp));
	if (!tmp)
		return NULL;

	tmp->vel    = 0;
	tmp->stvvel = 8;
	tmp->niz    = calloc(tmp->stvvel, sizeof(*tmp->niz));
	if (!tmp->niz) {
		free(tmp);
		return NULL;
	}
	return tmp;
}

void free_dniz(struct dniz *dniz) {
	free(dniz->niz);
	free(dniz);
}

int add_dniz(struct dniz *dniz, void *ul) {
	if (dniz->vel+1 >= dniz->stvvel) {
		void *t = realloc(dniz->niz, 2*(dniz->stvvel)*sizeof(void*));
		if (t) {
			dniz->stvvel *= 2;
			dniz->niz = t;
		} else {
			return -1;
		}
	}

	dniz->niz[dniz->vel++] = ul;

	return 0;
}


static uint64_t hes(const char *s) {
	const uint64_t p    = 31;
	const uint64_t m    = 1e9 + 9;
	uint64_t hash_value = 0;
	uint64_t p_pow = 1;
	for (char *c = (char*)s; *c; c++) {
		hash_value = (hash_value + (*c - 'a' + 1) * p_pow) % m;
		p_pow = (p_pow * p) % m;
	}
	return hash_value;
}

struct hmapa *new_hmapa() {
	return (struct hmapa*)new_dniz();
}

void free_hmapa(struct hmapa *mapa) {
	free_dniz((struct dniz*)mapa);
}

struct hmapa *add_hmapa(struct hmapa *mapa, const char *ime, void *pod) {
	if(mapa->niz.vel >= (mapa->niz.stvvel*3/4)) {
		void **t = calloc(mapa->niz.stvvel*2, sizeof(void*));
		if (!t)
			return NULL;

		struct hclan *t1;

		for(size_t i=0; i < mapa->niz.stvvel; i++) {
			if(!(t1=mapa->niz.niz[i]))
				continue;
			int ind;
			for(ind = t1->hesv % (mapa->niz.stvvel*2);
				t[ind]; ind = (ind+1)%(mapa->niz.stvvel*2));
			t[ind]=t1;
		}

		free(mapa->niz.niz);
		mapa->niz.niz = t;
		mapa->niz.stvvel *= 2;
	}

	struct hclan *hc = malloc(sizeof(*hc));

	if (!hc)
		return NULL;
	hc->ime  = strdup(ime);
	hc->hesv = hes(ime);
	hc->pok  = pod;

	int i = hc->hesv % mapa->niz.stvvel;
	for(; mapa->niz.niz[i]; i = (i+1)%mapa->niz.stvvel)
		if(!strcmp(ime, ((struct hclan*)(mapa->niz.niz[i]))->ime)) {
			free(hc->ime);
			free(hc);
			return NULL;
		}

	mapa->niz.niz[i]=hc;
	mapa->niz.vel++;

	return mapa;
}

struct hclan* find_hmapa(struct hmapa* h, const char* ime) {
	uint64_t hesv = hes(ime);
	size_t i = hesv % h->niz.stvvel, duz = 0;
	struct hclan* t;
	if(!(h->niz.niz[i]))
		return NULL;
	for(t = h->niz.niz[i];
			(t) && (duz < h->niz.stvvel) && (t->hesv != hesv);
			i++, t = h->niz.niz[i], duz++);

	if((t->hesv == hesv) && (!strcmp(t->ime, ime)))
		return t;

	return NULL;
}

void hstampaj(struct hmapa *mapa) {
	for(size_t i=0; i < mapa->niz.stvvel; i++) {
		printf("%4zd:\t", i);
		if(!mapa->niz.niz[i])
			printf("(null)\n");
		else {
			struct hclan *t=mapa->niz.niz[i];
			printf("\"%s\", heš: %lu, vred: %s\n", t->ime, t->hesv, (char*)t->pok);
		}
	}
}
