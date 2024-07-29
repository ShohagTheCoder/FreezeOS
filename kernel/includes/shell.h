#ifndef SHELL_H
#define SHELL_H

void execute_command(char buffer[]);
void command_help();
void command_ls();
void command_unknown(char buffer[]);
void shell(char c);
void shell_init();
void command_touch(char** commands);
void command_cat(char** cmds);
void command_sizeof(char** cmds);
void command_dump(char** cmds);

#endif