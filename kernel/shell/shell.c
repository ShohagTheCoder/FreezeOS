#include "../includes/shell.h"
#include "../includes/console.h"
#include "../includes/keyboard.h"
#include "../includes/string.h"

static char shell_buffer[256];

void shell_init()
{
    // Clear the screen and print welcome
    keyboard_bind(&shell);
    clear_screen();
    hr();
    print_str("Welcome to FreezeOS v1.0.3");
    hr();
    print_str("> ");
}

void shell(char c)
{
    if (c == '\b')
    {
        if (str_remove(shell_buffer))
        {
            cursor_back(1);
            putchar(' ');
            cursor_back(1);
        }
    }
    else if (c == '\n')
    {
        put_nl();
        execute_command(shell_buffer);
        print_str("> ");
    }
    else
    {
        str_append(shell_buffer, c);
        putchar(c);
    }
}

void execute_command(char buffer[])
{
    if (str_cmp(buffer, "help"))
    {
        command_help();
    }
    else if (str_cmp(buffer, "clear"))
    {
        clear_screen();
    }
    else
    {
        command_unknown(buffer);
    }

    // Reset the buffer
    reset_str(buffer);
}