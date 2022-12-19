#ifndef VM_ERROR_H
#define VM_ERROR_H

#ifndef DEBUG
#define DEBUG 0
#define dbg if (0)
#else
#define DEBUG 1
#define dbg log_stderr
#endif

enum vm_error {
    OK = 0,
    UNKNOWN_ERROR,
    FOPEN_ERROR,
    INPUT_FILE_READ_ERROR,
    OPT_PARSE_ERROR,
    MISALIGNED_INPUT_FILE_ERROR,
    MEMORY_ALLOCATION_ERROR,
    INVALID_OPCODE_ERROR,
    MPROTECT_ERROR,
    PC_OUT_OF_BOUNDS_ERROR,
    SP_OUT_OF_BOUNDS_ERROR,
    CP_OUT_OF_BOUNDS_ERROR
};

const char *vm_error_msg(enum vm_error rc);

void print_usage(void);

void log_stderr(const char *fmt, ...);

#endif /* VM_ERROR_H */
