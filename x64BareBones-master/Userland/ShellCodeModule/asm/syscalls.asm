GLOBAL sys_read
GLOBAL sys_write
GLOBAL sys_registers_snapshot
GLOBAL sys_set_font_size
GLOBAL sys_clear_screen
GLOBAL sys_put_pixel
GLOBAL sys_draw_rectangle
GLOBAL sys_draw_font
GLOBAL sys_set_mode
GLOBAL sys_sleep
GLOBAL sys_beep
GLOBAL sys_get_time
GLOBAL sys_get_ticks
GLOBAL sys_get_cpu_info

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

sys_registers_snapshot:
    syscall 2

sys_set_font_size:
    syscall 3

sys_clear_screen:
    syscall 4

sys_put_pixel:
    syscall 5

sys_draw_rectangle:
    syscall 6

sys_draw_font:
    syscall 7

sys_set_mode:
    syscall 8

sys_sleep:
    syscall 9

sys_beep:
    syscall 0xA

sys_get_time:
    syscall 0xB

sys_get_ticks:
    syscall 0xC

sys_get_cpu_info:
    syscall 0xD