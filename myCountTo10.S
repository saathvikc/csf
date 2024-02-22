    .section .rodata
sOutputFmt: .string "%ld\n"

    .section .text

    .globl main 
main:
    pushq %r12
    movq $1, %r12
.Ltop:
    movq $sOutputFmt, %rdi
    movq %r12, %rsi
    call printf
    incq %r12
    cmpq $10, %r12
    jle .Ltop

    popq %r12
    movl $0, %eax
    ret