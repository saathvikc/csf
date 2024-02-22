    .section .rodata
sPrompt:    .string "Enter Integer: "
sInputFmt:  .string "%ld"
sOutputFmt: .string "%ld\n"

    .section .text

    .globl main 
main:
    pushq %r12
    pushq %r13
    pushq %rbp
    subq $16, %rsp
    movq %rsp, %rbp

    movq $sPrompt, %rdi
    call printf

    movq $sInputFmt, %rdi
    movq %rbp, %rsi
    call scanf
    movq (%rbp), %r13
    movq $1, %r12

.Ltop:
    movq $sOutputFmt, %rdi
    movq %r12, %rsi
    call printf
    incq %r12
    cmpq %r13, %r12
    jle .Ltop

    addq $16, %rsp
    popq %r12
    popq %r13
    popq %rbp
    movl $0, %eax
    ret
