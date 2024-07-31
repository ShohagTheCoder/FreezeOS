#ifndef STRING_H
#define STRING_H

#include "types.h"

void print_str(char* str);
void print_str_in(char* str, int count);
size_t strlen(const char* s);
void int_to_ascii(int n, char str[]);
void print_int(int n);
void reverse_str(char* str);
int count_digits(int n);
void str_append(char str[], char n);
int str_remove(char str[]);
int str_cmp(char* str1, char* str2);
int str_cmp_in(char* str1, char* str2, int count);
void reset_str(char* str);
void fz_substr(char str[], char* substr, int start, int count);
void fz_strcpy(char* src, char* dest);
char* strcpy(char* dest, const char* src);
char* strcat(char* dest, const char* src);
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
