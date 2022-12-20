start:
    PUSH_I64 0

loop:
    PUSH_I64 10000000000
    JEQ done
    POP_I64
    INC_I64
    JMP loop

done:
    PRINT_I64
    HALT
