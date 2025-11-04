#include <utils.h>

uint64_t strlen(const char *s) {
    uint64_t i = 0;
    while (*s != 0) {
        i++;
        s++;
    }
    return i;
}

uint64_t strcmp(const char * str1, const char * str2) {
    int i = 0;
    while (str1[i] != 0 && str2[i] != 0) {
        if (str1[i] != str2[i]) {
            return str1[i] - str2[i];
        }
        i++;
    }
    return str1[i] - str2[i];
}

char * numToString(uint64_t num, uint64_t base) {
    static char buffer[64];
    char * ptr = &buffer[63];
    *ptr = '\0';
    do {
        *--ptr = "0123456789abcdef"[num % base];
        num /= base;
    } while(num != 0);
    return ptr;
}

int64_t puts(const char * str) {
    return write(STDOUT, str);
}

int64_t fputc(char c, uint64_t fd) {
    return write(fd, &c) == OK;
}

static int64_t vfprintf(uint64_t fd, const char *fmt, va_list args) {
    uint64_t flag = 0;
    uint64_t written = 0;

    for (uint64_t i = 0; fmt[i] != '\0'; i++) {
        if (fmt[i] == '%' && !flag) {
            flag = 1;
            i++;
        }

        if (!flag) {
            fputc(fmt[i], fd);
            flag = 0;
            written++;
            continue;
        }

        switch (fmt[i]) {
            case 'c':
                fputc(va_arg(args, int), fd);
                written++;
                break;
            case 'd':
                written += vfprintf(fd, numToString(va_arg(args, uint64_t), 10), args);
                break;
            case 'x':
                written += vfprintf(fd, "0x", args);
                written += vfprintf(fd, numToString(va_arg(args, uint64_t), 16), args);
                break;
            case 's':
                written += vfprintf(fd, va_arg(args, char *), args);
                break;
            case '%':
                fputc('%', fd);
                written++;
                break;
            default:
                return -1;
        }

        flag = 0;
    }

    return written;
}

// fprintf and printf were copied from this website http://www.firmcodes.com/write-printf-function-c/
int64_t fprintf(uint64_t fd, const char * fmt, ...) {
    va_list args;
    va_start(args, fmt);

    int64_t out = vfprintf(fd, fmt, args);

    va_end(args);
    return out;
}

int64_t printf(const char * fmt, ...) {
    va_list args;
    va_start(args, fmt);

    int64_t out = vfprintf(STDOUT, fmt, args);

    va_end(args);
    return out;
}

void printSnapshot() {
    CpuSnapshot snap;
    if(!getSnapshot(&snap)) {
        fprintf(STDERR, "No register snapshot available. Press ` to take a snapshot.\n");
        return;
    }

    puts("Register snapshot:\n");
    printf(" RAX: %x\n", snap.rax);
    printf(" RBX: %x\n", snap.rbx);
    printf(" RCX: %x\n", snap.rcx);
    printf(" RDX: %x\n", snap.rdx);
    printf(" RBP: %x\n", snap.rbp);
    printf(" RDI: %x\n", snap.rdi);
    printf(" RSI: %x\n", snap.rsi);
    printf(" R8:  %x\n", snap.r8 );
    printf(" R9:  %x\n", snap.r9 );
    printf(" R10: %x\n", snap.r10);
    printf(" R11: %x\n", snap.r11);
    printf(" R12: %x\n", snap.r12);
    printf(" R13: %x\n", snap.r13);
    printf(" R14: %x\n", snap.r14);
    printf(" R15: %x\n", snap.r15);
    printf(" RIP: %x\n", snap.rip);
    printf(" CS:  %x\n", snap.cs);
    printf(" RSP: %x\n", snap.rsp);
    printf(" SS:  %x\n", snap.ss);
}