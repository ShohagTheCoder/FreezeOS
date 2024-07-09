
#include <stdint.h>
#include <string.h>
#include "disk.h"
#include "fat12.h"

#pragma pack(push, 1)
typedef struct
{
    uint8_t jump[3];
    char oem[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fat_count;
    uint16_t root_entry_count;
    uint16_t total_sectors_16;
    uint8_t media_type;
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track;
    uint16_t head_count;
    uint32_t hidden_sectors;
    uint32_t total_sectors_32;
    uint8_t drive_number;
    uint8_t reserved;
    uint8_t boot_signature;
    uint32_t volume_id;
    char volume_label[11];
    char fs_type[8];
} __attribute__((packed)) FAT12_BPB;

typedef struct
{
    char filename[8];
    char extension[3];
    uint8_t attributes;
    uint8_t reserved;
    uint8_t creation_time_tenths;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t last_access_date;
    uint16_t first_cluster_high;
    uint16_t write_time;
    uint16_t write_date;
    uint16_t first_cluster_low;
    uint32_t file_size;
} __attribute__((packed)) FAT12_DirEntry;
#pragma pack(pop)

FAT12_BPB bpb;
FAT12_DirEntry root_dir[224];
uint16_t fat[2847];

void read_bpb()
{
    disk_read(0, sizeof(FAT12_BPB), &bpb);
}

void read_fat()
{
    disk_read(bpb.reserved_sectors, bpb.sectors_per_fat * bpb.bytes_per_sector, fat);
}

void read_root_dir()
{
    uint32_t root_dir_sector = bpb.reserved_sectors + (bpb.fat_count * bpb.sectors_per_fat);
    disk_read(root_dir_sector, sizeof(root_dir), root_dir);
}

uint16_t get_next_cluster(uint16_t cluster)
{
    uint16_t fat_index = cluster + (cluster / 2);
    uint16_t entry = *(uint16_t*)&((uint8_t*)fat)[fat_index];

    if (cluster & 0x0001)
        entry = entry >> 4;
    else
        entry = entry & 0x0FFF;

    return entry;
}

void read_cluster(uint16_t cluster, uint8_t* buffer)
{
    uint32_t first_data_sector = bpb.reserved_sectors + (bpb
