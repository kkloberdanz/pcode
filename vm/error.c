#include <stdarg.h>
#include <stdio.h>

#include "error.h"

static const char *const PROGRAM_NAME = "pvm";

const char *vm_error_msg(enum vm_error rc) {
    switch (rc) {
    case OK:
        return "OK";
    case UNKNOWN_ERROR:
        return "UNKNOWN_ERROR";
    case FOPEN_ERROR:
        return "FOPEN_ERROR";
    case INPUT_FILE_READ_ERROR:
        return "INPUT_FILE_READ_ERROR";
    case OPT_PARSE_ERROR:
        return "OPT_PARSE_ERROR";
    case MISALIGNED_INPUT_FILE_ERROR:
        return "MISALIGNED_INPUT_FILE_ERROR";
    case MEMORY_ALLOCATION_ERROR:
        return "MEMORY_ALLOCATION_ERROR";
    case INVALID_OPCODE_ERROR:
        return "INVALID_OPCODE_ERROR";
    case MPROTECT_ERROR:
        return "MPROTECT_ERROR";
    case PC_OUT_OF_BOUNDS_ERROR:
        return "PC_OUT_OF_BOUNDS_ERROR";
    case CP_OUT_OF_BOUNDS_ERROR:
        return "CP_OUT_OF_BOUNDS_ERROR";
    case SP_OUT_OF_BOUNDS_ERROR:
        return "SP_OUT_OF_BOUNDS_ERROR";
    default:
        return "unregistered error";
    }
}

void print_usage(void) {
    fprintf(
        stderr,
        "%s: reference implementation for pcode virtual machine\n"
        "usage:\n"
        "\t%s [OPTION] -- PROGRAM [ARGS]\n\n"
        "\t-h\tdisplay this help message\n\n",
        PROGRAM_NAME,
        PROGRAM_NAME
    );
}

void log_stderr(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "%s: ", PROGRAM_NAME);
    vfprintf(stderr, fmt, args); /* NOLINT */
    fprintf(stderr, "\n");
    va_end(args);
}
