#include <library.h>
#include <utils.h>

uint64_t read(uint64_t fd, char * buffer, uint64_t count){
    return sys_read(fd, buffer, count);   // guarda los valores de los parametros en los registros correspondientes
}

uint64_t write(uint64_t fd, const char * s){
    uint64_t len = strlen(s);
    return sys_write(fd, s, len);   // guarda los valores de los parametros en los registros correspondientes
}