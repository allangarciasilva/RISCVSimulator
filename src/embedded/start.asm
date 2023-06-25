.globl _start
.section .text.prologue

_start:
    nop
    auipc sp, %pcrel_hi(_stack_ptr)
    addi sp, sp, %lo(_stack_ptr)

    call start

    li a7, 10
    ecall