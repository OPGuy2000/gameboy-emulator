#include "memory.h"
#include "ppu.h"

unsigned char memory[MEMORY_SIZE];

unsigned char get_bit(unsigned char byte, int bit_position)
{
    return (byte >> bit_position) & 1;
}

unsigned char memory_read(unsigned short address)
{
    if (address < MEMORY_SIZE)
    {
        return memory[address];
    }
    return 0xFF; // Return 0xFF for out-of-bounds access
}

void memory_write(unsigned short address, unsigned char value)
{
    if (address < MEMORY_SIZE)
    {
        memory[address] = value;
    }
}
