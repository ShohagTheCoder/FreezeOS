#include "../includes/shell.h"
#include <stdio.h>
#include <string.h>
#include "../includes/console.h"
#include "../includes/keyboard.h"
#include "../includes/memory.h"
#include "../includes/string.h"

static char shell_buffer[256];

void shell_init()
{
    // Clear the screen and print welcome
    keyboard_bind(&shell);
    clear_screen();
    hr();
    puts("Welcome to FreezeOS v1.0.3");
    hr();
    puts("> ");
}

void shell(char c)
{
    if (c == '\b')
    {
        // If shell buffer is already empty then return;
        if (strlen(shell_buffer) == 0)
            return;

        pop(shell_buffer);
        cursor_back(1);
        putchar(' ');
        cursor_back(1);
    }
    else if (c == '\n')
    {
        put_nl();
        execute_command(shell_buffer);
        puts("> ");
    }
    else
    {
        push(shell_buffer, c);
        putchar(c);
    }
}

void execute_command(char* buffer)
{
    // puts(buffer);
    // put_nl();
    char len = strlen(buffer);
    char* temp = malloc(len);
    strcpy(temp, buffer);
    // puts(temp);
    // puts("------");

    char* command = strtok(temp, " ");
    char* data = strtok(NULL, "");

    // puts(command);
    // puts("|");
    // puts(data);
    // puts("|");

    if (strcmp("help", command) == 0)
    {
        command_help();
    }
    else if (strcmp("clear", command) == 0)
    {
        clear_screen();
    }
    else if (strcmp("ls", command) == 0)
    {
        command_ls();
    }
    else if (strcmp("touch", command) == 0)
    {
        command_touch(data);
    }
    else if (strcmp("echo", command) == 0)
    {
        command_echo(data);
    }
    else if (strcmp("cat", command) == 0)
    {
        command_cat(data);
    }
    else if (strcmp("sizeof", command) == 0)
    {
        command_sizeof(data);
    }
    else
    {
        // puts(command);
        command_unknown(command);
    }

    // Reset the buffer
    free(temp);
    memset(buffer, 0, len);
}
