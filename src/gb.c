#include <stdint.h>
#include <stdio.h>

#include "PPU/ppu.h"
#include "cartridge.h"
#include "memory.h"

uint8_t read_gb(uint16_t address) {
    if ((address >= 0x0000 && address <= 0x7FFF) || (address >= 0xA000 && address <= 0xBFFF)) {
        return mbc_read(address);
    } else {
        return mem_read(address);
    }
}

void write_gb(uint16_t address, uint8_t value) {
    if ((address >= 0x0000 && address <= 0x7FFF) || (address >= 0xA000 && address <= 0xBFFF)) {
        mbc_write(address, value);
    } else {
        mem_write(address, value);
    }
}

void init_gb(FILE *rom) {
    init_memory();

    load_cartridge(rom);
}

int main(int argc, char *argv[]) {
    FILE *rom;
    if (argc > 1) {
        rom = fopen(argv[1], "rb");
    } else {
        char fileName[100];
        printf("Enter file name of desired ROM: ");
        scanf("%s", fileName);
        rom = fopen(fileName, "rb");
    }
    init_gb(rom);

    return 0;
}
