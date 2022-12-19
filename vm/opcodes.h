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
    DEC,
    INC,
    JZ,
    JNZ,
    CALL,
    RET,
    JEQ,
    JLT,
    JLTE,
    JGT,
    JGTE
};

#endif /* VM_OPCODES_H */
