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
#define END_OF_CLUSTER 0xFFFF

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
    read_sectors((char*)root_entries, root_entries_start, 1);
}

void load_fat()
{
    read_sectors((char*)fat, fat_start, 1);
}

DirEntry_t find_file(char* name, char* extension)
{
    char fname[9] = "        ";
    char fextension[4] = "   ";
    to_uppercase(name);
    to_uppercase(extension);
    strncpy(fname, name, 8);
    strncpy(fextension, extension, 3);

    for (int i = 0; i < 256; i++)
    {
        if (strncmp(fname, (char*)root_entries[i].name, 8) &&
            strncmp(fextension, (char*)root_entries[i].extension, 3))
        {
            return root_entries[i];
        }
    }
    return empty_entry;
}

int get_next_cluster_number(int current_cluster)
{
    return ((int*)fat)[current_cluster];
}

uint32_t get_sector_number(int current_cluster, int add)
{
    return DATA_SECTOR_START + (current_cluster * 4) + add;
}

void load_file(char* buffer, DirEntry_t file)
{
    uint32_t size = (uint32_t)file.size;
    int current_cluster_number = file.first_cluster_low;
    int sector_count = 0;

    // Load clusters
    while (current_cluster_number != 0xff && size >= 2048)
    {
        load_cluster(buffer, get_sector_number(current_cluster_number, sector_count));
        size -= CLUSTER_SIZE;
        current_cluster_number = get_next_cluster_number(current_cluster_number);
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
    void* buffer = malloc((size_t)file.size);
    // Load the file data in buffer
    load_file(buffer, file);
    // Return buffer
    return buffer;
}

void fz_create_file(char name[], char extension[])
{
    char fname[9] = "        ";
    char fextension[4] = "   ";
    to_uppercase(name);
    to_uppercase(extension);
    printf("Name : %s, Extension : %s\n", name, extension);
    strncpy(fname, name, 8);
    strncpy(fextension, extension, 3);
    printf("Name : %s, Extension : %s\n", fname, fextension);

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
        if (fat[i] == 0)
        {
            cluster_position = i;
            break;
        }
    }

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
    fat[cluster_position] = 0xFFFF;

    // Copy contents
    strncpy((char*)root_entries[index].name, fname, 8);
    strncpy((char*)root_entries[index].extension, fextension, 3);

    // Write with new entry data
    fz_write_sector((char*)fat, fat_start);
    fz_write_sector((char*)root_entries, root_entries_start);
}

int get_file_index_in_root_directories(DirEntry_t file)
{
    int index = -1;
    for (int i = 0; i < 20; i++)
    {
        if (strncmp((const char*)file.name, (const char*)root_entries[i].name, 8) &&
            strncmp((const char*)file.extension, (const char*)root_entries[i].extension, 3))
        {
            index = i;
            break;
        }
    }

    return index;
}

void fz_fappend(DirEntry_t file, char* data, int length)
{
    if (length == NULL)
    {
        length = strlen(data);
    }

    // Variables
    int clusters_to_write = 0;
    int size = file.size;
    ClusterChain_t last_cluster = file.first_cluster_low;
    ClusterChain_t end_cluster = 0;
    int file_clusters = 0;

    size_t xs = size;
    // Find last sector
    while (xs >= CLUSTER_SIZE)
    {
        file_clusters++;
        xs -= CLUSTER_SIZE;
    }

    // Find how many sectors to write
    int last_data_offset = xs;
    xs += length;
    while (xs > 0)
    {
        clusters_to_write++;
        xs -= CLUSTER_SIZE;
    }

    end_cluster = fat[last_cluster];
    while (end_cluster != END_OF_CLUSTER)
    {
        last_cluster = end_cluster;
        end_cluster = fat[last_cluster];
    }

    // Read last cluster
    char* buffer = malloc(SECTOR_SIZE * clusters_to_write);
    // memset(buffer, 0, SECTOR_SIZE * clusters_to_write);

    load_cluster((char*)buffer, DATA_SECTOR_START + (last_cluster * SECTORS_PER_CLUSTER));
    strncpy(buffer + last_data_offset, data, length);

    // Write back last cluster with new data
    write_cluster((char*)buffer, DATA_SECTOR_START + (last_cluster * SECTORS_PER_CLUSTER));

    // Make free the memory
    free(buffer);

    int file_index = get_file_index_in_root_directories(file);

    // Update file and cluster chain
    root_entries[file_index].size += length;

    // Write with new entry data
    // fz_write_sector((char*)fat, fat_start);
    fz_write_sector((char*)root_entries, root_entries_start);
}

void fz_fdelete(DirEntry_t file)
{
    int file_index = get_file_index_in_root_directories(file);
    root_entries[file_index] = empty_entry;
    ClusterChain_t current_cluster = file.first_cluster_low;
    while (current_cluster != 0xFFFF)
    {
        ClusterChain_t next_culster = fat[current_cluster];
        fat[current_cluster] = 0;
        current_cluster = next_culster;
    }

    fz_write_sector((char*)fat, fat_start);
    fz_write_sector((char*)root_entries, root_entries_start);
}

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
