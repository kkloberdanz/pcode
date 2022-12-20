; TODO: This program will require some intermediate variable storage mechanism

start:
    PUSH_I64 97
    PUSH_I64 2      ; start with 2

loop:
    JEQ no          ; if we are at N, then it is not prime

    MOD_I64         ; check if candidate divides N
    PUSH_I64 0
    JEQ yes         ; if mod result is 0, then it divides, and we're done
    INC             ; otherwise, try the next value
    JMP loop

yes:
    PUSH 1
    PRINT_I64
    JMP end

no:
    PUSH 0
    PRINT_I64
    JMP end

end:
    HALT
