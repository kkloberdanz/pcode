start:
    PUSH_I64 1

loop:
    PUSH_I64 1000000000
    JEQ done
    POP_I64
    INC_I64
    JMP loop

done:
    PRINT_I64
    HALT
