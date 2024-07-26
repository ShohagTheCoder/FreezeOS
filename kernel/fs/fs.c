#include "../includes/fs.h"
#include "../includes/console.h"
#include "../includes/disk.h"
#include "../includes/io.h"
#include "../includes/memory.h"
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
#define SECTOR_SIZE 512
#define CLUSTER_SIZE 4 * SECTOR_SIZE
#define DATA_SECTOR_START 68

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

uint32_t fat_start = RESERVED_SECTORS;
uint32_t root_entries_start = RESERVED_SECTORS + (SECTORS_PER_FAT * 2);

void load_root_entries()
{
    read_sectors((uint8_t*)root_entries, root_entries_start, 1);
}

void load_fat()
{
    read_sectors((uint8_t*)fat, fat_start, 1);
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

uint16_t get_next_cluster_number(uint16_t current_cluster)
{
    return ((uint16_t*)fat)[current_cluster];
}

uint32_t get_sector_number(int current_cluster, int add)
{
    return DATA_SECTOR_START + (current_cluster * 4) + add;
}

void load_file(uint8_t* buffer, DirEntry_t file)
{
    uint32_t size = (uint32_t)file.size;
    uint8_t current_cluster_number = file.first_cluster_low;
    int sector_count = 0;

    // Load clusters
    while (current_cluster_number != 0xff && size >= 2048)
    {
        load_cluster(buffer, get_sector_number(current_cluster_number, sector_count));
        size -= CLUSTER_SIZE;
        current_cluster_number = (uint8_t)get_next_cluster_number(current_cluster_number);
        buffer += CLUSTER_SIZE;
        sector_count += 4;
    }

    // Load sectors
    int i = 0;
    while (size >= 512)
    {
        read_sector(buffer, get_sector_number(current_cluster_number, sector_count));
        size -= SECTOR_SIZE;
        buffer += SECTOR_SIZE;
        sector_count++;
        i++;
    }

    // Load bytes
    if (size > 0)
    {
        read_bytes(buffer, get_sector_number(current_cluster_number, sector_count), 0, size);
    }
}

void* file_read(char* name, char* ext)
{
    // Find file
    DirEntry_t file = find_file(name, ext);
    // Create buffer to hold the file
    void* buffer = mem_alloc((size_t)file.size);
    // Load the file data in buffer
    load_file(buffer, file);
    // Return buffer
    return buffer;
}

char* fz_make_fat16_file_name(char name[])
{
    // if (strlen(name) > 8)
    // {
    //     /* code */
    // }
    // else if (strlen(name) < 8)
    // {
    // }
    // else
    // {
    //     return name
    // }
}

void fz_create_file(char name[], char extension[])
{
    int index = 0;
    // Find empty file entry
    for (int i = 0; i < 255; i++)
    {
        if (root_entries[i].first_cluster_low == 0)
        {
            index = i;
            break;
        }
    }

    // Find emty cluster chain
    int cluster_position = 0;
    for (int i = 3; i < 512; i++)
    {
        if (fat[i].position == 0)
        {
            cluster_position = i;
            break;
        }
    }

    print_int(cluster_position);

    // Save file informations
    root_entries[index] = (DirEntry_t){
        .extra = "--------------------------------",
        .name = "        ",
        .extension = "   ",
        .attribute = 1,
        .first_cluster_high = 0,
        .first_cluster_low = cluster_position,
        .size = 0,
    };

    // Update fat
    fat[cluster_position].position = 0xFFFF;

    // Copy contents
    fz_substr(name, root_entries[index].name, 0, 8);
    fz_substr(extension, root_entries[index].extension, 0, 3);

    // Write with new entry data
    fz_write_sector(fat, fat_start);
    fz_write_sector(root_entries, root_entries_start);
}