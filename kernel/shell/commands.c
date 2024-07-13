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
    for (int i = 0; i < 5; i++)
    {
        // DirEntry_t entry = root_entries[i];
        // print_str(entry.name);
        print_str(" - ");
        print_int(i);
        // print_str(entry.extension);
        put_nl();
    }
}

void command_unknown(char buffer[])
{
    print_str("The command '");
    print_str(buffer);
    print_str("' is unknown.");
    put_nl();
}