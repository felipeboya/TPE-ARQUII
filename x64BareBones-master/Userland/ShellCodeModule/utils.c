#include <utils.h>
#include <status.h>

uint64_t strlen(const char *s) {
    uint64_t i = 0;
    while (*s != 0) {
        i++;
        s++;
    }
    return i;
}

uint64_t strcmp(const char * str1, const char * str2) {
    for(int i=0, j=0; str1[i] != 0 || str2[j] != 0; i++, j++) {
        if(str1[i] != str2[j]) {
            return str1[i] - str2[j];
        }
    }
    return OK;
}