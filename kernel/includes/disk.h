#ifndef DISK_H
#define DISK_H

#include "types.h"

void ata_wait_busy();
void ata_wait_drq();

void read_sectors(void* buffer, uint32_t lba, uint8_t sector_count);

#endif