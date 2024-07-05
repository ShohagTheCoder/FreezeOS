#ifndef STRING_H
#define STRING_H

#include "types.h"

void print_str(char* str);
int strlen(char* str);
void int_to_ascii(int n, char str[]);
void print_int(int n);
void reverse_str(char* str);
int count_digits(int n);
void str_append(char str[], char n);
int str_remove(char str[]);
int str_cmp(char* str1, char* str2);
void reset_str(char* str);

#endif
