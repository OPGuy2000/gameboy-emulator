#include "memory.h"

static unsigned char memory[MEMORY_SIZE];

unsigned char read_byte(unsigned short address) {
    if (address < MEMORY_SIZE) {
        return memory[address];
    }
    return 0xFF; // Return 0xFF for out-of-bounds access
}

void write_byte(unsigned short address, unsigned char value) {
    if (address < MEMORY_SIZE) {
        memory[address] = value;
    }
}

void init_memory() {
    // Initialize Boot Memory
    write_byte(0xFF00, 0xCF); // P1/JOYP
    write_byte(0xFF01, 0x00); // SB
    write_byte(0xFF02, 0x7E); // SC
    write_byte(0xFF04, 0x2B); // DIV
    write_byte(0xFF05, 0x00); // TIMA
    write_byte(0xFF06, 0x00); // TMA
    write_byte(0xFF07, 0xF8); // TAC
    write_byte(0xFF0F, 0xE1); // IF
    write_byte(0xFF10, 0x80); // NR10
    write_byte(0xFF11, 0xBF); // NR11
    write_byte(0xFF12, 0xF3); // NR12
    write_byte(0xFF13, 0xFF); // NR13
    write_byte(0xFF14, 0xBF); // NR14
    write_byte(0xFF16, 0x3F); // NR21
    write_byte(0xFF17, 0x00); // NR22
    write_byte(0xFF18, 0xFF); // NR23
    write_byte(0xFF19, 0xBF); // NR24
    write_byte(0xFF1A, 0x7F); // NR30
    write_byte(0xFF1B, 0xFF); // NR31
    write_byte(0xFF1C, 0x9F); // NR32
    write_byte(0xFF1D, 0xFF); // NR33
    write_byte(0xFF1E, 0xBF); // NR34
    write_byte(0xFF20, 0xFF); // NR41
    write_byte(0xFF21, 0x00); // NR42
    write_byte(0xFF22, 0x00); // NR43
    write_byte(0xFF23, 0xBF); // NR44
    write_byte(0xFF24, 0x77); // NR50
    write_byte(0xFF25, 0xF3); // NR51
    write_byte(0xFF26, 0xF1); // NR52
    write_byte(0xFF40, 0x91); // LCDC
    write_byte(0xFF41, 0x81); // STAT
    write_byte(0xFF42, 0x00); // SCY
    write_byte(0xFF43, 0x00); // SCX
    write_byte(0xFF44, 0x91); // LY
    write_byte(0xFF45, 0x00); // LYC
    write_byte(0xFF46, 0xFF); // DMA
    write_byte(0xFF47, 0xFC); // BGP
    write_byte(0xFF48, 0xFF); // OBP0
    write_byte(0xFF49, 0xFF); // OBP1
    write_byte(0xFF4A, 0x00); // WY
    write_byte(0xFF4B, 0x00); // WX
    write_byte(0xFFFF, 0x00); // IE
}
