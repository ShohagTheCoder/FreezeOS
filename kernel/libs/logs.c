#include "../includes/logs.h"
#include "../includes/disk.h"
#include "../includes/fs.h"
#include "../includes/memory.h"
#include "../includes/string.h"

void log_init()
{
    fz_create_file("errors", "txt");
    fz_create_file("warnings", "txt");
    fz_create_file("info", "txt");
}

void log(DirEntry_t file, char* message)
{
    char data[90];
    fz_strncpy("File name : ", data, 13);
    fz_strncpy("kernel.c", NULL, 10);
    fz_strncpy("\nFunction name : ", NULL, 18);
    fz_strncpy("main", NULL, 5);
    fz_strncpy("\nmessage : ", NULL, 11);
    fz_strcpy(message, NULL);

    fz_fappend(file, data, 90);
}

void log_warning(char* message)
{
    DirEntry_t file = find_file("warnings", "txt");

    log(file, message);
}

void log_error(char* message)
{
    DirEntry_t file = find_file("errors", "txt");

    print_str(file.name);

    log(file, message);
}

void log_info(char* message)
{
    DirEntry_t file = find_file("info", "txt");

    log(file, message);
}