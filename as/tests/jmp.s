start:
    PUSH_I64 1

loop:
    PRINT_I64 ; test comment
    INC_I64
    PUSH_I64 5
    JEQ done
    POP_I64
    JMP loop
    NOP

done:
    HALT
