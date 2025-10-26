global sys_write 

section .text

sys_write: 
    push rbp
    mov rbp, rsp

    mov rax, 1
    int 80h

    mov rsp, rbp
    pop rbp
    ret