#ifndef STRING_H
#define STRING_H

#include "types.h"

void print_str(char* str);
void print_str_in(char* str, int count);
int strlen(char* str);
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
void fz_fill_spaces(char* str, int end);

#endif
