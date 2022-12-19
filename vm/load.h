#ifndef VM_LOAD_H
#define VM_LOAD_H

#include <stdio.h>

#include "error.h"
#include "types.h"

enum vm_error vm_load(u64 **program, size_t *len, FILE *fp);
void vm_unload(u64 **program);

#endif /* VM_LOAD_H */
