#include "../includes/disk.h"
#include "../includes/io.h"
#include "../includes/string.h"

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

static inline void insl(uint16_t port, void* address, int cnt)
{
    asm volatile("cld; rep insl"
                 : "=D"(address), "=c"(cnt)
                 : "d"(port), "0"(address), "1"(cnt)
                 : "memory", "cc");
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

void read_sectors(void* buffer, uint32_t lba, uint8_t sector_count)
{
    outb(ATA_PRIMARY_CONTROL, 0x02);  // Disable IRQ's

    outb(ATA_PRIMARY_IO + ATA_SECTOR_COUNT, sector_count);
    outb(ATA_PRIMARY_IO + ATA_SECTOR_NUMBER, (uint8_t)lba);
    outb(ATA_PRIMARY_IO + ATA_CYLINDER_LOW, (uint8_t)(lba >> 8));
    outb(ATA_PRIMARY_IO + ATA_CYLINDER_HIGH, (uint8_t)(lba >> 16));
    outb(ATA_PRIMARY_IO + ATA_DRIVE_SELECT, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_PRIMARY_IO + ATA_COMMAND, ATA_CMD_READ_PIO);

    for (uint8_t i = 0; i < sector_count; i++)
    {
        ata_wait_busy();
        ata_wait_drq();
        insl(ATA_PRIMARY_IO + ATA_DATA, buffer, 256);  // 256 Words
        buffer = buffer + 512;                         // Advance the buffer pointer by 512 bytes
    }
}
