#ifndef SHELL_H
#define SHELL_H

void execute_command(char buffer[]);
void command_help();
void command_unknown(char buffer[]);
void shell(char c);
void shell_init();

#endif