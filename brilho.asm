segment .data
    v dd 75,75,75,0

segment .bss
    v1 resd 20000000

section .text
global brilho
brilho:


    push rbp
    mov rbp, rsp
    xor rcx, rcx
    mov rbx, v
    
    laco:
    cmp rcx, rdi
    jge fim
    
    movups xmm0, [rsi+rcx*4]
    movups xmm1, [rbx]
    addps xmm0, xmm1
    movups [v1+rcx*4], xmm0
    mov rax, v1
    
    add rcx, 4
    jmp laco
    fim:



leave
ret
