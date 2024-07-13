#include "../includes/fs.h"
#include "../includes/console.h"
#include "../includes/disk.h"
#include "../includes/io.h"
#include "../includes/string.h"

// Constants
#define SECTOR_SIZE 512
#define BYTES_PER_SECTOR 512
#define SECTORS_PER_CLUSTER 4
#define RESERVED_SECTORS 4
#define NUMBER_OF_FATS 2
#define ROOT_ENTRIES 512
#define ROOT_ENTRIES_SIZE 32
#define TOTAL_SECTORS 20480
#define SECTORS_PER_FAT 20

DirEntry_t root_entries[256];
ClusterChain_t fat[512];

DirEntry_t empty_entry = {.extra = {0},
                          .name = {0},
                          .extension = {0},
                          .attribute = 0,
                          .reserved = 0,
                          .create_time_tenth = 0,
                          .create_time = 0,
                          .create_date = 0,
                          .last_access_date = 0,
                          .first_cluster_high = 0,
                          .write_time = 0,
                          .write_date = 0,
                          .first_cluster_low = 0,
                          .size = 0};

void load_root_entries()
{
    uint32_t root_entries_start = RESERVED_SECTORS + (SECTORS_PER_FAT * 2);
    read_sectors(root_entries, root_entries_start, 1);
    putchar('C');
}

void load_fat()
{
    uint32_t fat_start = RESERVED_SECTORS;
    read_sectors(fat, fat_start, 1);
}

DirEntry_t find_file(char name[], char extension[])
{
    for (int i = 0; i < 256; i++)
    {
        if (str_cmp_in(name, (char*)root_entries[i].name, 8) &&
            str_cmp_in(extension, (char*)root_entries[i].extension, 3))
        {
            return root_entries[i];
        }
    }
    return empty_entry;
}
