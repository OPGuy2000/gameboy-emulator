#include <stdint.h>
#include <stdio.h>

#include "PPU/ppu.h"
#include "cartridge.h"
#include "memory.h"

uint8_t bus_read(uint16_t address) {
    uint8_t ppu_mode = get_mode();
    if (ppu_mode == 2) {
        if (address >= OAM_START && address <= OAM_END)
            return 0xFF;
    } else if (ppu_mode == 3) {
        return 0xFF;
    }

    if ((address >= 0x0000 && address <= 0x7FFF) || (address >= 0xA000 && address <= 0xBFFF)) {
        return mbc_read(address);
    } else {
        return mem_read(address);
    }
}

void bus_write(uint16_t address, uint8_t value) {
    uint8_t ppu_mode = get_mode();
    if (ppu_mode == 2) {
        if (address >= OAM_START && address <= OAM_END)
            return;
    } else if (ppu_mode == 3) {
        return;
    }

    if ((address >= 0x0000 && address <= 0x7FFF) || (address >= 0xA000 && address <= 0xBFFF)) {
        mbc_write(address, value);
    } else {
        mem_write(address, value);
    }
}

void gb_init(FILE *rom) {
    mem_init();

    cartridge_load(rom);
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

    mem_init();

    gb_init(rom);

    ppu_init(mem_read, mem_write);

    return 0;
}
