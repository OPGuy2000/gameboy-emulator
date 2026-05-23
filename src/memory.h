#pragma once

#ifndef MEMORY_H
#define MEMORY_H

#include <stdbool.h>
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

union MBC {
    struct MBC1_State {
        unsigned char rom_bank;
        unsigned char upper_bits;
        unsigned char ram_bank;
        unsigned char banking_mode;
        bool ram_enabled;
    } MBC1;
};

unsigned char mem_read(unsigned short address);
void mem_write(unsigned short address, unsigned char value);
void mbc_write(unsigned short address, unsigned char value);
int init_memory(FILE *romfp);

unsigned char get_mbc();

#endif
