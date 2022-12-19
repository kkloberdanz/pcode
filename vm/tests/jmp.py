add_opcode = bytes([6,0,0,0,0,0,0,0])
push_opcode = bytes([3,0,0,0,0,0,0,0])
halt_opcode = bytes([2,0,0,0,0,0,0,0])
print_i64_opcode = bytes([5,0,0,0,0,0,0,0])
jmp_opcode = bytes([10,0,0,0,0,0,0,0])
dec_opcode = bytes([11,0,0,0,0,0,0,0])
jz_opcode = bytes([13,0,0,0,0,0,0,0])
jnz_opcode = bytes([14,0,0,0,0,0,0,0])

f = open('jmp.bin', 'wb')

f.write(push_opcode)
f.write(bytes([10,0,0,0,0,0,0,0]))

f.write(print_i64_opcode)

f.write(dec_opcode)

f.write(jnz_opcode)
f.write(bytes([2,0,0,0,0,0,0,0]))

f.write(halt_opcode)

f.close()
