start:
    READ_I64        ; store number to compute N! of at location 0
    STORE_I64 0     ; location 0 is the accumulated multiplication
    STORE_I64 1     ; location 1 is the decrementing count

loop:
    LOAD_I64 1      ; load the decrementing count
    PUSH_I64 1      ; once N is 1, then we are done
    JEQ print       ; then print the result
    POP_I64         ; otherwise, pop the '1', and continue

    DEC_I64         ; decrement count by 1

    STORE_I64 1     ; store the count
    LOAD_I64 0      ; load accumulator to top of stack
    MUL_I64         ; multiply the accumulator by the current count
    STORE_I64 0     ; store multiplication result in accumulator
    JMP loop        ; repeat until top of stack is 1

print:
    LOAD_I64 0      ; place result at top of stack
    PRINT_I64       ; print the result
    JMP end         ; and we're done

end:
    HALT            ; goodbye
