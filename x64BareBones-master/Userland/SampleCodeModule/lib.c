#include <library.h>

uint64_t write(uint64_t fd, const char * s){
    sys_write(fd, s, 28);   // guarda los valores de los parametros en los registros correspondientes
}