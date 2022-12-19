/*
 * (C) 2022 Kyle Kloberdanz
 *
 * Reference implementation for Kyle's pcode
 */

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "exec.h"
#include "opcodes.h"
#include "types.h"
#include "vm_getopt.h"

struct vm_opts {
    const char *input_file;
};

static void vm_opts_init(struct vm_opts *opts) {
    opts->input_file = NULL;
}

static int parse_opts(
    int argc, char *const *const argv, struct vm_opts *opts /* out */
) {
    int c;
    while ((c = vm_getopt(argc, argv, "h")) != -1) {
        switch (c) {
        case 'h':
            print_usage();
            exit(EXIT_SUCCESS);
        default:
            return -1;
        }
    }
    if (vm_optind < argc) {
        opts->input_file = argv[vm_optind];
        return vm_optind;
    } else {
        return 0;
    }
}

int main(int argc, char **argv) {
    int index;
    enum vm_error rc;
    struct vm_opts opts;
    u64 exit_code;
    FILE *input_file = NULL;

    vm_opts_init(&opts);

    index = parse_opts(argc, argv, &opts);
    if (index < 0) {
        rc = OPT_PARSE_ERROR;
        goto cleanup;
    }

    if (opts.input_file) {
        input_file = fopen(opts.input_file, "rb");
        if (!input_file) {
            perror("fopen");
            rc = FOPEN_ERROR;
            goto cleanup;
        }
    } else {
        /* read from stdin */
        input_file = stdin;
    }

    rc = vm_run(input_file, &exit_code);
    if (rc) {
        goto cleanup;
    }

cleanup:
    if (input_file) {
        fclose(input_file);
    }
    if (rc) {
        log_stderr("error: (%d) %s", rc, vm_error_msg(rc));
    } else {
        dbg("success: program exited with code: %" PRIu64, exit_code);
    }
    return rc;
}
