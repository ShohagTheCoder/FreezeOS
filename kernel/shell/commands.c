#include "../includes/console.h"
#include "../includes/fs.h"
#include "../includes/memory.h"
#include "../includes/shell.h"
#include "../includes/string.h"

#define KiB 1042

void command_help()
{
    puts("List of commands:\n");
    puts("- ls    : List of files\n");
    puts("- clear : Clear the screen\n");
    puts("- echo  : Print something on console\n");
    puts("- mkdir : Make a new directory\n");
    puts("- cd    : Change directory\n");
}

void command_ls()
{
    extern DirEntry_t root_entries[256];

    puts("-------------------------------\n");
    puts(" Name     | Ext | Size\n");
    puts("-------------------------------\n");

    for (int i = 0; i < 256; i++)
    {
        if (strlen((char*)root_entries[i].name) > 0)
        {
            putchar(' ');
            DirEntry_t entry = root_entries[i];
            putns((char*)entry.name, 8);
            puts(" | ");
            putns((char*)entry.extension, 3);
            puts(" | ");
            if (entry.size < KiB)
            {
                puti(entry.size);
                puts(" byte");
            }
            else
            {
                puti(entry.size / KiB);
                puts(" KiB");
            }
            put_nl();
        }
    }
}

// The command is unkonwn
void command_unknown(char buffer[])
{
    printf("The command %s is unknown.\n", buffer);
}

void command_touch(char** cmds)
{
    to_uppercase(cmds[1]);
    to_uppercase(cmds[2]);
    fz_create_file(cmds[1], cmds[2]);
    put_nl();
}

void command_cat(char** cmds)
{
    DirEntry_t file = find_file(cmds[1], cmds[2]);

    fz_fappend(file, cmds[3]);
    printf("Data append successfull on file : %s . %s\n", cmds[1], cmds[2]);
}

void command_sizeof(char** cmds)
{
    DirEntry_t file = find_file(cmds[1], cmds[2]);
    puts("File name : ");
    putns((char*)file.name, 8);
    printf("File size : %n\n", file.size);
}
void command_dump(char** cmds)
{
    DirEntry_t file = find_file(cmds[1], cmds[2]);
    char* buffer = malloc(file.size);
    load_file(buffer, file);
    puts("File Contents : ");
    putns(buffer, file.size);
    put_nl();
    free(buffer);
}