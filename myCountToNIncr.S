    .section .rodata
sPrompt1:   .string "Enter Maximum: "
sPrompt2:   .string "Enter Increment: "
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

    movq $sPrompt1, %rdi
    call printf

    movq $sInputFmt, %rdi
    leaq 8(%rbp), %rsi
    call scanf

    movq $sPrompt2, %rdi
    call printf

    movq $sInputFmt, %rdi
    leaq 0(%rbp), %rsi
    call scanf

    movq 8(%rbp), %r13
    movq $1, %r12

.Ltop:
    movq $sOutputFmt, %rdi
    movq %r12, %rsi
    call printf
    addq 0(%rbp), %r12
    cmpq %r13, %r12
    jle .Ltop

    addq $16, %rsp
    popq %r12
    popq %r13
    popq %rbp
    movl $0, %eax
    ret