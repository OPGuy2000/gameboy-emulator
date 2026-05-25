#include "cartridge.h"

static uint8_t *rom;
static uint8_t *bank0;
static uint8_t *bankn;

static uint8_t *cart_ram;
static uint8_t *ram_bank;

static struct MBC mbc;
static uint16_t num_banks;
static uint8_t num_ram_banks;

uint8_t mbc_read(uint16_t address) {
    switch (mbc.mbc_type) {
        // No MBC
    case 0x00: {
        return 0xFF;
    }
    // MBC1
    case 0x01:
    case 0x02:
    case 0x03: {
        if (address <= 0x3FFF) {
            // Bank 0
            return bank0[address];
        } else if (address <= 0x7FFF) {
            // Bank n
            return bankn[address - 0x4000];
        } else if (address >= 0xA000 && address <= 0xBFFF) {
            // External RAM
            if (!mbc.mbc_data.MBC1.ram_enabled || num_ram_banks == 0)
                return 0xFF;
            return ram_bank[address - 0xA000];
        }

        printf("Error: Out of bounds address given to MBC1!");
        return 0xFF;
    }
    default:
        printf("Error: MBC not supported!");
        return 0xFF;
    }
}

void mbc_write(uint16_t address, uint8_t value) {
    union MBCData *mbcData = &mbc.mbc_data;
    switch (mbc.mbc_type) {
    // No MBC
    case 0x00: {
        return;
    }
    // MBC1
    case 0x01:
    case 0x02:
    case 0x03: {
        struct MBC1_State *mbc1 = &mbcData->MBC1;
        if (address <= 0x1FFF) {
            // Enable/Disable RAM
            mbc1->ram_enabled = (value & 0x0F) == 0x0A;
        } else if (address <= 0x3FFF) {
            // Chooses bank n (in 16kB increments)
            uint8_t bank = value & 0x1F;
            if (bank == 0x00)
                bank = 0x01;

            if (num_banks > 0x1F) {
                bank |= (mbc1->ram_bank << 5);
            }

            bank &= (num_banks - 1);
            bankn = rom + bank * 0x4000;
        } else if (address <= 0x5FFF) {
            // Chooses RAM bank + upper 2 bits of most addresses
            bankn -= mbc1->ram_bank * 0x40000;
            mbc1->ram_bank = value & 0x03;
            bankn = cart_ram + mbc1->ram_bank * 0x40000;
            if (mbc1->banking_mode == 1) {
                bank0 = rom + mbc1->ram_bank * 0x40000;
                ram_bank = cart_ram + mbc1->ram_bank * 0x2000;
            }
        } else if (address <= 0x7FFF) {
            // Chooses banking mode
            mbc1->banking_mode = value & 0x01;

            if (mbc1->banking_mode == 0) {
                bank0 = rom;
                ram_bank = cart_ram;
            } else {
                bank0 = rom + mbc1->ram_bank * 0x40000;
                ram_bank = cart_ram + mbc1->ram_bank * 0x2000;
            }
        }
        break;
    }
    default:
        printf("Error: MBC not supported!");
        return;
    }
}

void load_cartridge(FILE *romfp) {
    // Initialize and ROM
    if (romfp == NULL) {
        printf("ERROR: Game ROM could not be opened.");
        return;
    }

    fseek(romfp, 0, SEEK_END);
    long romSize = ftell(romfp);
    num_banks = romSize / 0x4000;
    rewind(romfp);

    rom = malloc(romSize);
    fread(rom, 1, romSize, romfp);

    fclose(romfp);

    // Copy over Bank 0 and 1 to memory
    bank0 = rom;
    bankn = rom + 0x4000;

    // Initialize MBC
    mbc.mbc_type = rom[0x0147];
    switch (mbc.mbc_type) {
    case 0x01:
    case 0x02:
    case 0x03: {
        mbc.mbc_data.MBC1 =
            (struct MBC1_State){.ram_bank = 0x00, .banking_mode = 0x00, .ram_enabled = false};
    }; break;
    default:
        printf("Error: MBC not supported!");
        return;
    }

    switch (rom[0x0149]) {
    case 0x00: {
        num_ram_banks = 0;
        break;
    }
    case 0x02: {
        num_ram_banks = 1;
        break;
    }
    case 0x03: {
        num_ram_banks = 4;
        break;
    }
    default:
        printf("Error: MBC not supported!");
        return;
    }

    if (num_ram_banks > 0) {
        cart_ram = malloc(num_ram_banks * 0x2000);
        memset(cart_ram, 0xFF, num_ram_banks * 0x2000);

        ram_bank = cart_ram;
    }

    char title[16];
    memcpy(title, rom + 0x0134, 16);
    printf("Title of ROM: %s", title);
}

struct MBC get_mbc() { return mbc; }
