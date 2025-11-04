GLOBAL _hlt
GLOBAL opCode
GLOBAL div0

section .text
	
_hlt:
	sti
	hlt
	ret

opCode:
    push rbp
    mov rbp, rsp

    jmp 0

    mov rsp, rbp
    pop rbp
    ret

div0:
    push rbp
    mov rbp, rsp

    mov rdx, 1
    mov rax, 0
    div rax

    mov rsp, rbp
    pop rbp
    ret