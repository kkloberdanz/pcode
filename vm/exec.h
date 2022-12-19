#ifndef VM_EXEC_H
#define VM_EXEC_H

#include <stdint.h>

#include "error.h"
#include "types.h"

enum vm_error vm_run(FILE *input_file, u64 *exit_code);

#endif /* VM_EXEC_H */
