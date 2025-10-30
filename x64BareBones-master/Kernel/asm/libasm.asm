GLOBAL cpuVendor, getKey, rtc, outb, inb

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

getKey:
    push rbp
    mov rbp, rsp

    mov rax, 0
	in al, 0x60

    leave
    ret

rtc:
    push rbp
    mov rbp, rsp

    mov al, dil
    out 70h, al
    in al, 71h

    mov rsp, rbp
    pop rbp
    ret

; void outb(uint16_t port, uint8_t value)
outb:
	push rdx

    mov dx, di
    mov al, sil
	out dx, al

    pop rdx
    ret

; extern uint8_t inb(uint16_t port)
inb:
    push rdx

    mov dx, di
    in al, dx

    pop rdx
    ret