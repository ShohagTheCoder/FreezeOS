#ifndef STRING_H
#define STRING_H

#include "types.h"

// Define va_list and va_* macros
typedef char* va_list;

#define va_start(ap, last) (ap = (va_list)&last + sizeof(last))
#define va_args(ap, type) (*(type*)((ap += sizeof(type)) - sizeof(type)))
#define va_end(ap) (ap = (va_list)0);
#define MAX_BUFFER_SIZE 1024

// Standard funcitons
size_t strlen(const char* s);
void strrev(char* str);
void itoa(int num, char* str, int base);
int strcmp(const char* a, const char* b);
int strncmp(const char* a, const char* b, size_t n);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);
char* strcat(char* dest, const char* src);
void pop(char* str);
void push(char* str, char c);
void puts(const char* str);
void putns(const char* str, size_t n);
void printf(const char* format, ...);
char* strchr(const char* str, int c);
char* strpbrk(const char* str, const char* accept);
size_t strspn(const char* str, char* accept);
char* strtok(char* str, const char* delim);
void strsplit(char* str, char** pointers, char* delim);
void puti(int n);
int count_digits(int n);
void to_uppercase(char* str);
void to_lowercase(char* str);
char* substr(const char* src, int start, int length);

#endif
