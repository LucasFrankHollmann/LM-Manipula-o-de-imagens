segment .data
    noise dd 33
    i dd 0

segment .bss
    v1 resd 20000000
    v resq 20000000

section .text
extern rand
extern printf
global ruido
ruido:


    push rbp
    mov rbp, rsp
    
    xor rcx, rcx
    mov [i], rdi
    
    push rdi
    push rsi
    loop1:
    push rcx
    call rand
    pop rcx
    xor rdx, rdx
    div dword [noise]
    mov [v+rcx*4], edx
    inc rcx
    
    
    push rcx
    call rand
    pop rcx
    xor rdx, rdx
    div dword [noise]
    mov [v+rcx*4], edx
    inc rcx
    
    
    push rcx
    call rand
    pop rcx
    xor rdx, rdx
    div dword [noise]
    mov [v+rcx*4], edx
    inc rcx
    cmp rcx, [i]
    
    
    mov dword [v+rcx*4], 0

    cmp rcx, [i]
    jl loop1
    
    pop rsi
    pop rdi
    
    
    xor rcx, rcx
    mov rbx, v
    laco:
    cmp rcx, [i]
    jge fim
    mov rbx, v
    movups xmm0, [rsi+rcx*4]
    movups xmm1, [rbx+rcx*4]
    addps xmm0, xmm1
    movups [v1+rcx*4], xmm0
    
    
    add rcx, 4
    jmp laco
    fim:
    mov rax, v1


leave
ret

