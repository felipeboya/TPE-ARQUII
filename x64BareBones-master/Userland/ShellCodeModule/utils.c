#include <utils.h>

uint64_t strlen(const char *s) {
    uint64_t i = 0;
    while (*s != 0) {
        i++;
        s++;
    }
    return i;
}
