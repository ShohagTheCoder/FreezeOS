#ifndef CONSOLE_H
#define CONSOLE_H

void move_cursor();
void clear_screen();
void putchar(char c);
void put_nl();
void cursor_back(int count);
void hr();
void s(char* str);
void n(int n);
void d(char* str, int n);

#endif