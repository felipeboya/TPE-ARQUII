#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <library.h>
#include <status.h>
#include <stdarg.h>

uint64_t strlen(const char * s);
uint64_t strcmp(const char * str1, const char * str2);
char * numToString(uint64_t num, uint64_t base);
int64_t puts(const char * str);
int64_t fputc(char c, uint64_t fd);
int64_t fprintf(uint64_t fd, const char * fmt, ...);
int64_t printf(const char * fmt, ...);

#endif