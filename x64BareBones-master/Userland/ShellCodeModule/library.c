#include <library.h>
#include <utils.h>

// Declaramos las funciones ASM que hacen el int 0x80
extern uint64_t sys_read(uint64_t fd, char *buffer, uint64_t size);
extern uint64_t sys_write(uint64_t fd, const char *buffer, uint64_t size);

uint64_t read(uint64_t fd, char * buffer, uint64_t count){
    return sys_read(fd, buffer, count);   // guarda los valores de los parametros en los registros correspondientes
}

uint64_t write(uint64_t fd, const char * s){
    uint64_t len = strlen(s);
    return sys_write(fd, s, len);   // guarda los valores de los parametros en los registros correspondientes
}

char getchar(void){
    uint16_t c;
    while(read(0, &c, 1)==0 || c>255);
    return (char) c;
}


putchar
scanf
printf

