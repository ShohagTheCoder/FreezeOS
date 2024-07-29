#ifndef DISK_H
#define DISK_H

#include "types.h"

void ata_wait_busy();
void ata_wait_drq();

void read_sector(uint8_t* buffer, uint32_t lba);
void read_sectors(uint8_t* buffer, uint32_t lba, uint8_t sector_count);
void read_bytes(uint8_t* buffer, uint32_t lba, uint16_t offset, uint16_t bytes_count);
void load_cluster(uint8_t* buffer, uint32_t lba);

void fz_write_sector(uint8_t* buffer, uint32_t lba);
void fz_write_sectors(uint8_t* buffer, uint32_t lba, uint8_t sector_count);
void write_cluster(uint8_t* buffer, uint32_t lba);

#endif