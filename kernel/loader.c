#include "includes/loader.h"
#include "includes/types.h"

void load_kernel()
{
    volatile char* vid_mem = (volatile char*)0xb8000;
    vid_mem[0] = 'L';
    vid_mem[1] = 0x2f;

    print_string('K');

    while (1)
    {
        /* code */
    }
}

void print_string(char c)
{
    volatile char* vid_mem = (volatile char*)0xb8000;
    vid_mem[20] = c;
    vid_mem[21] = 0x4f;
}
