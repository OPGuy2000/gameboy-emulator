#pragma once

#ifndef MEMORY_H
#define MEMORY_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_SIZE 0x4000

// Memory segmentation
#define ROM_BANK_0_START 0x0000
#define ROM_BANK_0_END 0x3FFF

#define ROM_BANK_N_START 0x4000
#define ROM_BANK_N_END 0x7FFF

#define VRAM_START 0x8000
#define VRAM_END 0x9FFF

#define EXTERNAL_RAM_START 0xA000
#define EXTERNAL_RAM_END 0xBFFF

#define WORK_RAM_START 0xC000
#define WORK_RAM_END 0xDFFF

#define ECHO_RAM_START 0xE000
#define ECHO_RAM_END 0xFDFF

#define OAM_START 0xFE00
#define OAM_END 0xFE9F

#define IO_REGISTERS_START 0xFF00
#define IO_REGISTERS_END 0xFF7F

#define HIGH_RAM_START 0xFF80
#define HIGH_RAM_END 0xFFFE

union MBCData {
    struct MBC1_State {
        uint8_t rom_bank;
        uint8_t ram_bank;
        uint8_t banking_mode;
        bool ram_enabled;
    } MBC1;
};
struct MBC {
    uint8_t mbc_type;
    union MBCData mbc_data;
};

uint8_t mem_read(uint16_t address);
void mem_write(uint16_t address, uint8_t value);
uint8_t mbc_read(uint16_t address);
void mbc_write(uint16_t address, uint8_t value);

void init_memory(FILE *romfp);

struct MBC get_mbc();

#endif
