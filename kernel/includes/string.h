#ifndef STRING_H
#define STRING_H

#include "types.h"

// Define va_list and va_* macros
typedef char* va_list;

#define va_start(ap, last) (ap = (va_list)&last + sizeof(last))
#define va_args(ap, type) (*(type*)((ap += sizeof(type)) - sizeof(type)))
#define va_end(ap) (ap = (va_list)0);

// Standard funcitons
size_t strlen(const char* s);
void strrev(char* str);
void itoa(int num, char* str, int base);
int strcmp(const char* a, const char* b);
int strncmp(const char* a, const char* b, size_t n);
char* strcpy(char* dest, const char* src);
char* strcat(char* dest, const char* src);

void print_str(char* str);
void print_str_in(char* str, int count);
void print_int(int n);
void reverse_str(char* str);
int count_digits(int n);
void str_append(char str[], char n);
int str_remove(char str[]);
void printf(const char* format, ...);

void reset_str(char* str);
void fz_substr(char str[], char* substr, int start, int count);
void fz_strcpy(char* src, char* dest);

void fz_strncpy(char* src, char* dest, int count);
void fz_strcpy_max(char* src, char* dest, int max);
void fz_fill_spaces(char* str, int end);
void fz_to_uppercase(char* str);
void fz_to_lowercase(char* str);

char* fz_strchr(const char* str, int c);
char* fz_strpbrk(const char* str, const char* accept);
size_t fz_strspn(const char* str, char* accept);
char* fz_strtok(char* str, const char* delim);

void fz_strsplit(char* str, char** pointers, char* delim);

#endif
