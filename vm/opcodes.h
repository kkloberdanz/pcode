#ifndef VM_OPCODES_H
#define VM_OPCODES_H

enum vm_opcodes {
    BRK = 0, /* software interrupt */
    NOP,     /* No Op */
    HALT,
    PUSH_I64,
    POP_I64,
    PRINT_I64,
    ADD_I64,
    SUB_I64,
    MUL_I64,
    DIV_I64,
    JMP,
    DEC_I64,
    INC_I64,
    JZ,
    JNZ,
    CALL,
    RET,
    JEQ,
    JLT,
    JLTE,
    JGT,
    JGTE,
    SQRT_I64,
    READ_I64,
    MOD_I64,
    DUP_I64,
    STORE_I64,
    LOAD_I64
};

#endif /* VM_OPCODES_H */
