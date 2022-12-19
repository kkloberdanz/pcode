#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "vec.h"

void vm_vec_init(struct vm_vec *vec) {
    vec->data = NULL;
    vec->len = 0;
    vec->cap = 0;
}

int vm_vec_push(struct vm_vec *vec, u64 data) {
    if (vec->cap == 0) {
        u64 *new_buf = malloc(sizeof(*new_buf));
        if (!new_buf) {
            perror("malloc");
            goto fail;
        }
        vec->cap = 1;
        vec->data = new_buf;
    } else if (vec->len >= vec->cap) {
        size_t new_cap = vec->cap * 2;
        u64 *new_buf = NULL;
        new_buf = realloc(vec->data, new_cap * sizeof(*new_buf));
        if (!new_buf) {
            perror("realloc");
            goto fail;
        }
        vec->data = new_buf;
        vec->cap = new_cap;
    }

    vec->data[vec->len] = data;
    vec->len++;
    return 0;

fail:
    return -1;
}

void vm_vec_free(struct vm_vec *vec) {
    if (vec) {
        free(vec->data);
        vec->data = NULL;
        vec->cap = 0;
        vec->len = 0;
    }
}
