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
    char* commnads[5];
    fz_strsplit(buffer, commnads, " ");

    if (str_cmp("help", buffer))
    {
        command_help();
    }
    else if (str_cmp("clear", buffer))
    {
        clear_screen();
    }
    else if (str_cmp("ls", buffer))
    {
        command_ls();
    }
    else if (str_cmp("touch", buffer))
    {
        command_touch(commnads);
    }
    else if (str_cmp("cat", buffer))
    {
        command_cat(commnads);
    }
    else if (str_cmp("sizeof", buffer))
    {
        command_sizeof(commnads);
    }
    else if (str_cmp("dump", buffer))
    {
        command_dump(commnads);
    }
    else
    {
        command_unknown(buffer);
    }

    // Reset the buffer
    reset_str(buffer);
}