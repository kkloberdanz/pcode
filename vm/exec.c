#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "error.h"
#include "exec.h"
#include "load.h"
#include "opcodes.h"
#include "types.h"

static void print_stack(const u64 *stack, size_t sp) {
    size_t i;
    if (!stack) {
        return;
    }
    if (!DEBUG) {
        return;
    }
    fprintf(stderr, "\n*** PRINTING STACK ***\n");
    for (i = 0; i < sp; i++) {
        fprintf(stderr, "0x%" PRIx64 "\n", stack[i]);
    }
    fprintf(stderr, "*** DONE PRINTING STACK ***\n\n");
}

static void print_program(const u64 *program, size_t program_len) {
    size_t i;
    if (!DEBUG) {
        return;
    }
    if (!program) {
        return;
    }
    fprintf(stderr, "\n*** PRINTING PROGRAM ***\n");
    for (i = 0; i < program_len; i++) {
        fprintf(stderr, "0x%" PRIx64 "\n", program[i]);
    }
    fprintf(stderr, "*** DONE PRINTING PROGRAM ***\n\n");
}

#define DEC(I)                                                                \
    do {                                                                      \
        if ((I) == 0) {                                                       \
            rc = I##_OUT_OF_BOUNDS_ERROR;                                     \
            log_stderr(                                                       \
                #I " decremented out of bounds on line %lu", __LINE__         \
            );                                                                \
            goto done;                                                        \
        } else {                                                              \
            (I)--;                                                            \
        }                                                                     \
    } while (0)

#define INC(I, MAX)                                                           \
    do {                                                                      \
        if ((I) > MAX) {                                                      \
            rc = I##_OUT_OF_BOUNDS_ERROR;                                     \
            log_stderr(                                                       \
                #I " incremented out of bounds on line %lu", __LINE__         \
            );                                                                \
            goto done;                                                        \
        } else {                                                              \
            (I)++;                                                            \
        }                                                                     \
    } while (0)

/*
 * TODO:
 * - Add LOCAL_I64 opcode to make room for local variable on the call stack
 * - When CALL instruction executes
 *   + set BP := CP
 *   + push return address on call address
 *   + push BP on call stack
 * - When RET instruction executes
 *   + pop BP from call stack
 *   + set CP := BP
 *   + jump to call_stack[CP]
 * - When LOCAL_I64 instruction executes, INC(CP)
 * - Add LOAD_LOCAL_I64, which takes the offset from BP as an argument
 * - Add STORE_LOCAL_I64, which takes the offset from BP as an argument and the
 *   value to store
 */

/*
 * Setup stack and heap memory.
 * Execute program.
 */
enum vm_error vm_run(FILE *input_file, u64 *exit_code) {
    enum vm_error rc = OK;
    enum vm_opcodes opcode;
    size_t PC = 0;
    size_t SP = 0;
    size_t CP = 0;
    u64 *stack = NULL;
    u64 *call_stack = NULL;
    size_t stack_len = 10000000;
    size_t call_stack_len = 10000000;
    u64 *program = NULL;
    size_t program_len = 0;

    rc = vm_load(&program, &program_len, input_file);
    if (rc) {
        goto done;
    }

    stack = malloc(stack_len * sizeof(stack));
    if (!stack) {
        perror("malloc");
        rc = MEMORY_ALLOCATION_ERROR;
        goto done;
    }

    call_stack = malloc(call_stack_len * sizeof(call_stack));
    if (!call_stack) {
        perror("malloc");
        rc = MEMORY_ALLOCATION_ERROR;
        goto done;
    }

    /* This is the hot path.
     * Use goto to ensure best possible performance on any platform */
top:

    print_stack(stack, SP);
    if (PC > program_len) {
        log_stderr("program counter out of bounds");
        rc = PC_OUT_OF_BOUNDS_ERROR;
        goto done;
    }
    opcode = program[PC];
    dbg("OPCODE: %lx\n", opcode);
    INC(PC, program_len);
    switch (opcode) {
    case PUSH_I64:
        dbg("PUSH_I64 %llx", program[PC]);
        stack[SP] = program[PC];
        INC(PC, program_len);
        INC(SP, stack_len);
        break;

    case ADD_I64: {
        i64 a, b;
        DEC(SP);
        a = stack[SP];
        DEC(SP);
        b = stack[SP];
        dbg("ADD_I64 %llx %llx", a, b);
        stack[SP] = a + b;
        INC(SP, stack_len);
        break;
    }

    case SUB_I64: {
        i64 a, b;
        DEC(SP);
        a = stack[SP];
        DEC(SP);
        b = stack[SP];
        dbg("SUB_I64 %llx %llx", a, b);
        stack[SP] = a - b;
        INC(SP, stack_len);
        break;
    }

    case MUL_I64: {
        i64 a, b;
        DEC(SP);
        a = stack[SP];
        DEC(SP);
        b = stack[SP];
        stack[SP] = a * b;
        INC(SP, stack_len);
        break;
    }

    case DIV_I64: {
        i64 a, b;
        DEC(SP);
        a = stack[SP];
        DEC(SP);
        b = stack[SP];
        stack[SP] = a / b;
        INC(SP, stack_len);
        break;
    }

    case POP_I64:
        DEC(SP);
        break;

    case PRINT_I64:
        if (SP == 0) {
            rc = SP_OUT_OF_BOUNDS_ERROR;
            goto done;
        }
        dbg("PRINT_I64 %llx", stack[SP - 1]);
        printf("%" PRIu64 "\n", stack[SP - 1]);
        break;

    case JMP:
        PC = program[PC];
        dbg("JMP %lx", PC);
        break;

    case CALL:
        call_stack[CP] = PC + 1;
        PC = program[PC];
        INC(CP, call_stack_len);
        break;

    case RET:
        DEC(CP);
        PC = call_stack[CP];
        break;

    case JZ:
        if (SP == 0) {
            rc = SP_OUT_OF_BOUNDS_ERROR;
            goto done;
        }
        if (stack[SP - 1] == 0) {
            PC = program[PC];
        } else {
            INC(PC, program_len);
        }
        break;

    case JNZ:
        if (SP == 0) {
            rc = SP_OUT_OF_BOUNDS_ERROR;
            goto done;
        }
        if (stack[SP - 1] != 0) {
            PC = program[PC];
        } else {
            INC(PC, program_len);
        }
        break;

    case INC:
        if (SP == 0) {
            rc = SP_OUT_OF_BOUNDS_ERROR;
            goto done;
        }
        stack[SP - 1]++;
        dbg("INC (%llx)", stack[SP - 1]);
        break;

    case DEC:
        if (SP == 0) {
            rc = SP_OUT_OF_BOUNDS_ERROR;
            goto done;
        }
        stack[SP - 1]--;
        break;

    case JEQ: {
        u64 a, b;
        DEC(SP);
        b = stack[SP];
        DEC(SP);
        a = stack[SP];
        INC(SP, stack_len);
        dbg("JEQ:  0x%llx == 0x%llx ? (0x%lx)", a, b, program[PC]);
        if (a == b) {
            PC = program[PC];
        } else {
            INC(PC, program_len);
        }
        break;
    }

    case JLT: {
        u64 a, b;
        DEC(SP);
        b = stack[SP];
        DEC(SP);
        a = stack[SP];
        if (a < b) {
            PC = program[PC];
        }
        break;
    }

    case JLTE: {
        u64 a, b;
        DEC(SP);
        b = stack[SP];
        DEC(SP);
        a = stack[SP];
        if (a <= b) {
            PC = program[PC];
        }
        break;
    }

    case JGT: {
        u64 a, b;
        DEC(SP);
        b = stack[SP];
        DEC(SP);
        a = stack[SP];
        if (a > b) {
            PC = program[PC];
        }
        break;
    }

    case JGTE: {
        u64 a, b;
        DEC(SP);
        b = stack[SP];
        DEC(SP);
        a = stack[SP];
        if (a >= b) {
            PC = program[PC];
        }
        break;
    }

    case NOP:
        goto top;

    case BRK: /* TODO: handle interrupts */
    case HALT:
        if (SP == 0) {
            rc = SP_OUT_OF_BOUNDS_ERROR;
            goto done;
        }
        rc = OK;
        *exit_code = stack[SP - 1];
        goto done;

    default:
        log_stderr("invalid opcode: 0x%" PRIx64, opcode);
        rc = INVALID_OPCODE_ERROR;
        goto done;
    }
    goto top;
done:
    print_stack(stack, SP);
    free(stack);
    free(call_stack);
    print_program(program, program_len);
    if (program) {
        vm_unload(&program);
    }
    return rc;
}
