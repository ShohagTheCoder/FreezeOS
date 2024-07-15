#include "includes/loader.h"
#include "includes/types.h"

// Constants
#define ATA_PRIMARY_IO 0x1F0
#define ATA_PRIMARY_CONTROL 0x3F6
#define ATA_MASTER 0xA0
#define ATA_SLAVE 0xB0
#define ATA_SECTOR_COUNT 0x02
#define ATA_SECTOR_NUMBER 0x03
#define ATA_CYLINDER_LOW 0x04
#define ATA_CYLINDER_HIGH 0x05
#define ATA_DRIVE_SELECT 0x06
#define ATA_COMMAND 0x07
#define ATA_DATA 0x00
#define ATA_STATUS 0x07
#define ATA_STATUS_BUSY 0x80
#define ATA_STATUS_DRQ 0x08
#define ATA_CMD_READ_PIO 0x20

void load_kernel()
{
    volatile char* vid_mem = (volatile char*)0xb8000;
    vid_mem[0] = 'L';
    vid_mem[1] = 0x2f;

    print_string('K');

    load_root_entries();
    load_fat();

    DirEntry_t kernel_file = find_file("KERNEL  ", "BIN");

    // vid_mem[2] = (char)kernel_file.name[0];
    // vid_mem[3] = 0x2f;

    uint8_t* kernel_buffer = (uint8_t*)0x100000;

    load_file(kernel_buffer, kernel_file);

    void (*kernel)() = (void (*)())kernel_buffer;

    kernel();

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

// Recive a byte to an I/O port
uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile("inb %1, %0"
                 : "=a"(ret)  // Return value will be store in ret variable
                 : "Nd"(port));

    return ret;
}

// Send a byte to an I/O port
void outb(uint16_t port, uint8_t val)
{
    asm volatile("outb %0, %1"
                 :  // No output
                 : "a"(val), "Nd"(port));
}

void insl(uint16_t port, void* address, int cnt)
{
    asm volatile("cld; rep insl"
                 : "=D"(address), "=c"(cnt)
                 : "d"(port), "0"(address), "1"(cnt)
                 : "memory", "cc");
}

int str_cmp_in(char* str1, char* str2, int count)
{
    while (count && *str1 && (*str1 == *str2))
    {
        count--;
        str1++;
        str2++;
    }
    return count == 0 ? 1 : 0;
}

void ata_wait_busy()
{
    while (inb(ATA_PRIMARY_IO + ATA_STATUS) & ATA_STATUS_BUSY)
        ;
}

void ata_wait_drq()
{
    while (!(inb(ATA_PRIMARY_IO + ATA_STATUS) & ATA_STATUS_DRQ))
        ;
}

void read_sector(uint8_t* buffer, uint32_t lba)
{
    outb(ATA_PRIMARY_CONTROL, 0x02);  // Disable IRQ's

    outb(ATA_PRIMARY_IO + ATA_SECTOR_COUNT, 1);
    outb(ATA_PRIMARY_IO + ATA_SECTOR_NUMBER, (uint8_t)lba);
    outb(ATA_PRIMARY_IO + ATA_CYLINDER_LOW, (uint8_t)(lba >> 8));
    outb(ATA_PRIMARY_IO + ATA_CYLINDER_HIGH, (uint8_t)(lba >> 16));
    outb(ATA_PRIMARY_IO + ATA_DRIVE_SELECT, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_PRIMARY_IO + ATA_COMMAND, ATA_CMD_READ_PIO);

    // Load the data
    ata_wait_busy();
    ata_wait_drq();
    insl(ATA_PRIMARY_IO + ATA_DATA, buffer, 256);  // 256 Words
}

void read_sectors(uint8_t* buffer, uint32_t lba, uint8_t sector_count)
{
    for (uint8_t i = 0; i < sector_count; i++)
    {
        read_sector(buffer, lba + i);
        buffer = buffer + 512;  // Advance the buffer pointer by 512 bytes
    }
}

void read_bytes(uint8_t* buffer, uint32_t lba, uint16_t offset, uint16_t bytes_count)
{
    uint8_t* placeholder[512];

    outb(ATA_PRIMARY_CONTROL, 0x02);  // Disable IRQ's

    outb(ATA_PRIMARY_IO + ATA_SECTOR_COUNT, 1);
    outb(ATA_PRIMARY_IO + ATA_SECTOR_NUMBER, (uint8_t)lba);
    outb(ATA_PRIMARY_IO + ATA_CYLINDER_LOW, (uint8_t)(lba >> 8));
    outb(ATA_PRIMARY_IO + ATA_CYLINDER_HIGH, (uint8_t)(lba >> 16));
    outb(ATA_PRIMARY_IO + ATA_DRIVE_SELECT, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_PRIMARY_IO + ATA_COMMAND, ATA_CMD_READ_PIO);

    // Load the data
    ata_wait_busy();
    ata_wait_drq();
    insl(ATA_PRIMARY_IO + ATA_DATA, placeholder, 256);  // 256 Words

    for (uint16_t i = 0; i <= bytes_count; i++)
    {
        buffer[i] = ((uint8_t*)placeholder)[offset + i];
    }
}

void load_cluster(uint8_t* buffer, uint32_t lba)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        read_sector(buffer, lba + i);
        buffer = buffer + 512;  // Advance the buffer pointer by 512 bytes
    }
}

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

void load_root_entries()
{
    uint32_t root_entries_start = RESERVED_SECTORS + (SECTORS_PER_FAT * 2);
    read_sectors((uint8_t*)root_entries, root_entries_start, 1);
}

void load_fat()
{
    uint32_t fat_start = RESERVED_SECTORS;
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