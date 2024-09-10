addi x1, a0, 0
addi x2, x0, 1
addi x3, x0, 1

loop:
    and x2, x1, x3
    add a1, x2, a1
    srl x1, x1, x3
    bne x1, x0, loop