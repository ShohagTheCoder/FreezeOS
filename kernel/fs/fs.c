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
    read_sectors((uint8_t*)root_entries, root_entries_start, 1);
}

void load_fat()
{
    read_sectors((uint8_t*)fat, fat_start, 1);
}

DirEntry_t find_file(char name[], char extension[])
{
    char fname[8] = "ONE     ";
    char fextension[3] = "TXT";
    // char* fname = mem_alloc(8);
    // memset(fname, ' ', 8);
    // char* fextension = mem_alloc(3);
    // memset(fextension, ' ', 3);
    fz_to_uppercase(name);
    fz_to_uppercase(extension);
    // fz_strcpy_max(name, fname, 8);
    // fz_strcpy_max(extension, fextension, 3);
    // char* fextension = "TXT";

    print_str(fname);
    print_str(fextension);
    print_str(" |");

    for (int i = 0; i < 256; i++)
    {
        if (str_cmp_in(fname, (char*)root_entries[i].name, 8) &&
            str_cmp_in(fextension, (char*)root_entries[i].extension, 3))
        {
            mem_free(fname);
            mem_free(extension);
            return root_entries[i];
        }
    }
    mem_free(fname);
    mem_free(extension);
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

char* fz_make_fat16_file_name(char* name, char* extension)
{
    char* full_name = "           ";  // 11 characters name (name + extension)
    fz_to_uppercase(name);
    fz_to_uppercase(extension);
    fz_strcpy_max(name, full_name, 8);
    fz_strcpy_max(extension, full_name + 8, 3);
    // Return the new name
    return full_name;
}

void fz_create_file(char name[], char extension[])
{
    char* fname = "           ";
    char* fextension = "   ";
    fz_to_uppercase(name);
    fz_to_uppercase(extension);
    fz_strcpy_max(name, fname, 8);
    fz_strcpy_max(extension, fextension, 3);

    print_str(fname);

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
    fz_strcpy_max(fname, (char*)root_entries[index].name, 8);
    fz_strcpy_max(fextension, (char*)root_entries[index].extension, 3);

    // Write with new entry data
    fz_write_sector((uint8_t*)fat, fat_start);
    fz_write_sector((uint8_t*)root_entries, root_entries_start);

    // File created successfully
    print_str("File created successfully\n");
}

int get_file_index_in_root_directories(DirEntry_t file)
{
    int index = -1;
    for (int i = 0; i < 20; i++)
    {
        if (str_cmp_in(file.name, root_entries[i].name, 8) &&
            str_cmp_in(file.extension, root_entries[i].extension, 3))
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

    int xs = size;
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
    char* buffer = mem_alloc(SECTOR_SIZE * clusters_to_write);
    // memset(buffer, 0, SECTOR_SIZE * clusters_to_write);

    load_cluster((uint8_t*)buffer, DATA_SECTOR_START + (last_cluster * SECTORS_PER_CLUSTER));
    fz_strncpy(data, buffer + last_data_offset, length);

    // Write back last cluster with new data
    write_cluster((uint8_t*)buffer, DATA_SECTOR_START + (last_cluster * SECTORS_PER_CLUSTER));

    // Make free the memory
    mem_free(buffer);

    int file_index = get_file_index_in_root_directories(file);

    // Update file and cluster chain
    root_entries[file_index].size += length;

    // Write with new entry data
    // fz_write_sector((uint8_t*)fat, fat_start);
    fz_write_sector((uint8_t*)root_entries, root_entries_start);
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

    fz_write_sector((uint8_t*)fat, fat_start);
    fz_write_sector((uint8_t*)root_entries, root_entries_start);
}