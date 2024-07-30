#ifndef TYPES_H
#define TYPES_H

#ifndef NULL
#define NULL 0
#endif

typedef enum
{
    true = 1,
    false = 0,
} bool;

typedef enum
{
    ERROR_LOG = 1,
    WARNING_LOG = 2,
    INFO_LOG = 3,
} log_types;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long size_t;
typedef unsigned long long uint64_t;

#endif