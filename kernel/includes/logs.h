#ifndef LOGS_H
#define LOGS_H

#include "types.h"

void log_init();

void log_warning(char* message);
void log_error(char* message);
void log_info(char* message);

#endif