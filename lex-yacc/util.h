#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
#include <stdlib.h>

struct dniz {
    size_t vel;
    size_t stvvel;
    void **niz;
};

/* Funckije */

struct dniz *new_dniz();
void free_dniz(struct dniz *dniz);
int add_dniz(struct dniz *dniz, void *ul);

#endif /* UTIL_H */
