#include "util.h"

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
