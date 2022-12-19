#ifndef VEC_H
#define VEC_H

#include "types.h"

struct vm_vec {
    u64 *data;
    size_t len;
    size_t cap;
};

void vm_vec_init(struct vm_vec *vec);

int vm_vec_push(struct vm_vec *vec, u64 data);

void vm_vec_free(struct vm_vec *vec);

#endif /* VEC_H */
