#ifdef __linux__
#define _GNU_SOURCE
#endif

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "load.h"
#include "types.h"
#include "vec.h"

#define UNUSED(A) (void)(A)

static size_t nbytes = 0;
static size_t program_len = 0;

static int check_little_endian(void) {
    volatile u32 i = 0x01234567;
    /* return 0 for big endian, 1 for little endian. */
    return (*((volatile uint8_t *)(&i))) == 0x67;
}

static void swap_endianness_32_bit(u32 *value) {
    /* 32 bit swap */
    u32 tmp = ((*value << 8) & 0xFF00FF00) | ((*value >> 8) & 0xFF00FF);
    *value = (tmp << 16) | (tmp >> 16);
}

static void swap_endianness_64_bit(u64 *value) {
    UNUSED(swap_endianness_32_bit);
#if defined(__GNUC__)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlong-long"

#elif defined(__clang__)

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wlong-long"

#endif
    /* 64 bit swap */
    *value = ((*value & 0x00000000FFFFFFFFull) << 32) |
             ((*value & 0xFFFFFFFF00000000ull) >> 32);
    *value = ((*value & 0x0000FFFF0000FFFFull) << 16) |
             ((*value & 0xFFFF0000FFFF0000ull) >> 16);
    *value = ((*value & 0x00FF00FF00FF00FFull) << 8) |
             ((*value & 0xFF00FF00FF00FF00ull) >> 8);

#if defined(__GNUC__)

#pragma GCC diagnostic pop

#elif defined(__clang__)

#pragma clang diagnostic pop

#endif
}

static void *vm_program_alloc(size_t sz) {
    void *data = NULL;
#ifdef __WIN32__
    data = malloc(sz);
#else
    data = mmap(
        NULL, sz, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0
    );
    if (data == MAP_FAILED) {
        perror("mmap");
        data = NULL;
        goto done;
    }
#endif
done:
    return data;
}

static enum vm_error vm_program_protect(void *data, size_t sz) {
    enum vm_error rc = OK;
#ifndef __WIN32__
    if (mprotect(data, sz, PROT_READ)) {
        perror("mprotect");
        rc = MPROTECT_ERROR;
        goto done;
    }
#endif
done:
    return rc;
}

/*
 * Load binary pcode file into program memory.
 * Set program memory to read only. (mprotect)
 * TODO: make memory protection portable
 */
enum vm_error vm_load(u64 **program, size_t *len, FILE *fp) {
    struct vm_vec vec;
    enum vm_error rc = OK;
    int is_little_endian = check_little_endian();

    vm_vec_init(&vec);

    /* read 64 bits at a time into program */
    for (;;) {
        u64 word;
        size_t n = fread(&word, VM_WORD_SIZE, 1, fp);
        if (n != 1) {
            if (feof(fp)) {
                /* it's ok, we're at the end of the file */
                break;
            }

            if (ferror(fp)) {
                /* not good, we encountered an error */
                perror("fread");
                rc = INPUT_FILE_READ_ERROR;
                goto cleanup;
            }

            /* short read, not good */
            rc = MISALIGNED_INPUT_FILE_ERROR;
            goto cleanup;
        }

        if (!is_little_endian) {
            swap_endianness_64_bit(&word);
        }
        vm_vec_push(&vec, word);
        program_len++;
    }

    if (program_len == 0) {
        rc = MISALIGNED_INPUT_FILE_ERROR;
        goto cleanup;
    }

    nbytes = program_len * VM_WORD_SIZE;

    *program = vm_program_alloc(nbytes);
    if (*program == NULL) {
        rc = MEMORY_ALLOCATION_ERROR;
        goto cleanup;
    }

    memcpy(*program, vec.data, nbytes);

    /* protects the program memory so that writing to it will result in a
     * deadly signal (bus error) */
    if (vm_program_protect(*program, nbytes)) {
        perror("mprotect");
        rc = MPROTECT_ERROR;
        goto cleanup;
    }

cleanup:
    if (rc && program && *program) {
        munmap(*program, nbytes);
        *program = NULL;
    }
    *len = program_len;
    vm_vec_free(&vec);
    return rc;
}

void vm_unload(u64 **program) {
    if (program && *program) {
#ifdef __WIN32__
        free(*program);
        *program = NULL;
#else
        munmap(*program, nbytes);
        *program = NULL;
#endif
    }
}
