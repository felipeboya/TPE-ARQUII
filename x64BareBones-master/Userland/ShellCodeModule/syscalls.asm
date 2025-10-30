global sys_read
global sys_write 

section .text

%macro syscall 1
    push rbp
    mov rbp, rsp

    mov rax, %1
    int 80h

    mov rsp, rbp
    pop rbp
    ret
%endmacro

sys_read: 
    syscall 0

sys_write: 
    syscall 1