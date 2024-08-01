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

void save_log(DirEntry_t file, log_t log)
{
    // Allocate memory for buffer
    size_t log_size = strlen(log.file) + strlen(log.function) + strlen(log.message) + 1;
    char* buffer = malloc(log_size);
    strcat(buffer, "File : ");
    strcat(buffer, log.file);
    strcat(buffer, ", Function : ");
    strcat(buffer, log.function);
    strcat(buffer, ", Message : ");
    strcat(buffer, log.message);
    // Save the buffer to the log file
    fz_fappend(file, buffer, NULL);
    // Make free the memory
    free(buffer);
}

void WARN(char* file, char* function, char* message)
{
    log_t log = {.file = file, .function = function, .message = message};
    DirEntry_t log_file = find_file("warnings", "txt");
    save_log(log_file, log);
}

void ERROR(char* file, char* function, char* message)
{
    log_t log = {.file = file, .function = function, .message = message};
    DirEntry_t log_file = find_file("errors", "txt");
    save_log(log_file, log);
}

void INFO(char* file, char* function, char* message)
{
    log_t log = {.file = file, .function = function, .message = message};
    DirEntry_t log_file = find_file("info", "txt");
    save_log(log_file, log);
}