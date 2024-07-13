#include "../includes/console.h"
#include "../includes/fs.h"
#include "../includes/shell.h"
#include "../includes/string.h"

void command_help()
{
    print_str("List of commands:");
    put_nl();
    print_str("- ls    : List of files");
    put_nl();
    print_str("- clear : Clear the screen");
    put_nl();
    print_str("- echo  : Print something on console");
    put_nl();
    print_str("- mkdir : Make a new directory");
    put_nl();
    print_str("- cd    : Change directory");
    put_nl();
}

void command_ls()
{
    extern DirEntry_t root_entries[256];

    print_str("-------------------------------");
    put_nl();
    print_str(" Name     | Ext | Size");
    put_nl();
    print_str("-------------------------------");
    put_nl();

    for (int i = 0; i < 256; i++)
    {
        if (strlen(root_entries[i].name) > 0)
        {
            putchar(' ');
            DirEntry_t entry = root_entries[i];
            print_str_in(entry.name, 8);
            print_str(" | ");
            print_str_in(entry.extension, 3);
            print_str(" | ");
            print_int(entry.size);
            print_str(" (char)");
            put_nl();
        }
    }
}

void command_unknown(char buffer[])
{
    print_str("The command '");
    print_str(buffer);
    print_str("' is unknown.");
    put_nl();
}