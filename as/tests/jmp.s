start:
    PUSH_I64 1

loop:
    PRINT_I64 ; test comment
    INC
    PUSH_I64 5
    JEQ done
    JMP loop
    NOP

done:
    HALT
