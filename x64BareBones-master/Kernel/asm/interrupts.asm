GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler

GLOBAL _int80Handler

GLOBAL _exception0Handler
GLOBAL _exception6Handler

EXTERN irqDispatcher
EXTERN syscallDispatcher
EXTERN exceptionDispatcher

EXTERN registersArrayAux
EXTERN registersArrayException

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro popStateNoRax 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

%macro exceptionHandler 1
	cli
	pushState
	; guardamos los valores de los registros generales
	mov [registersArrayException], rax
    mov [registersArrayException + 8*1], rbx      ; rbx
    mov [registersArrayException + 8*2], rcx      ; rcx
    mov [registersArrayException + 8*3], rdx      ; rdx
    mov [registersArrayException + 8*4], rbp      ; rsi
    mov [registersArrayException + 8*5], rdi      ; rdi
    mov [registersArrayException + 8*6], rsi      ; rbp
    mov [registersArrayException + 8*7], r8       ; r8
    mov [registersArrayException + 8*8], r9       ; r9
    mov [registersArrayException + 8*9], r10      ; r10
    mov [registersArrayException + 8*10], r11     ; r11
    mov [registersArrayException + 8*11], r12     ; r12
    mov [registersArrayException + 8*12], r13     ; r13
    mov [registersArrayException + 8*13], r14     ; r14
    mov [registersArrayException + 8*14], r15     ; r15

    mov rax, [rsp + 8*15]                    ; RIP guardado por la CPU
    mov [registersArrayException + 8*15], rax      
	mov rax, [rsp + 8*16]					; CS guardado por la CPU
	mov [registersArrayException + 8*16], rax      
	mov rax, [rsp + 8*17] 				  	; RFLAGS guardado por la CPU
	mov [registersArrayException + 8*17], rax		  
	mov rax, [rsp + 8*18]				   	; RSP guardado por la CPU
	mov [registersArrayException + 8*18], rax    
	mov rax, [rsp + 8*19]					; SS guardado por la CPU
	mov [registersArrayException + 8*19], rax 

	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

	popState
	sti
	iretq
%endmacro

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret

_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn

; Hardware Interrupts

;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:

	push rax

	; ante cualquier interrupción de teclado guardamos todo el contexto del CPU
	; en caso de que el usuario pretenda realizar el snapshot del estado actual. 
	; la CPU pushea al stack automaticamente los registros RIP, CS, RFLAGS, RSP, SS

	; guardamos los valores de los registros generales
	mov [registersArrayAux], rax
    mov [registersArrayAux + 8*1], rbx      ; rbx
    mov [registersArrayAux + 8*2], rcx      ; rcx
    mov [registersArrayAux + 8*3], rdx      ; rdx
    mov [registersArrayAux + 8*4], rbp      ; rsi
    mov [registersArrayAux + 8*5], rdi      ; rdi
    mov [registersArrayAux + 8*6], rsi      ; rbp
    mov [registersArrayAux + 8*7], r8       ; r8
    mov [registersArrayAux + 8*8], r9       ; r9
    mov [registersArrayAux + 8*9], r10      ; r10
    mov [registersArrayAux + 8*10], r11     ; r11
    mov [registersArrayAux + 8*11], r12     ; r12
    mov [registersArrayAux + 8*12], r13     ; r13
    mov [registersArrayAux + 8*13], r14     ; r14
    mov [registersArrayAux + 8*14], r15     ; r15

    mov rax, [rsp + 8*1]                    ; RIP guardado por la CPU
    mov [registersArrayAux + 8*15], rax      
	mov rax, [rsp + 8*2]					; CS guardado por la CPU
	mov [registersArrayAux + 8*16], rax      
	mov rax, [rsp + 8*3] 				  	; RFLAGS guardado por la CPU
	mov [registersArrayAux + 8*17], rax		  
	mov rax, [rsp + 8*4]				   	; RSP guardado por la CPU
	mov [registersArrayAux + 8*18], rax    
	mov rax, [rsp + 8*5]					; SS guardado por la CPU
	mov [registersArrayAux + 8*19], rax     
	
	pop rax ; recupero rax

	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

; System Calls (Software Interrupts)

; calling conventions: https://wiki.osdev.org/Calling_Conventions

_int80Handler:
    pushState
	push r9		 	 ; (stack) arg6
    mov r9, r8       ; r9 = arg5
    mov r8, r10      ; r8 = arg4
    mov rcx, rdx     ; rcx = arg3
    mov rdx, rsi     ; rdx = arg2
    mov rsi, rdi     ; rsi = arg1
    mov rdi, rax     ; rdi = syscall_id 

    call syscallDispatcher

	add rsp, 8		; r9
	popStateNoRax	; popeamos todo menos rax
	add rsp, 8		; no recuperamos rax
	
    iretq

; Exceptions

;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

_exception6Handler:
	exceptionHandler 6

haltcpu:
	cli
	hlt
	ret

SECTION .bss
	aux resq 1