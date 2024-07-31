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
    s(message);
    fz_fappend(file, message, NULL);
}

void log_warning(char* message)
{
    DirEntry_t file = find_file("warnings", "txt");

    log(file, message);
}

void log_error(char* message)
{
    s(message);
    DirEntry_t file = find_file("errors", "txt");

    log(file, message);
}

void log_info(char* message)
{
    DirEntry_t file = find_file("info", "txt");

    log(file, message);
}