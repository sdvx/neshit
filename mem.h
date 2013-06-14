#ifndef _MEM_H
#define _MEM_H

#include "common.h"

#define MEM_READ(a)     mem.read(a)
#define MEM_WRITE(a, b) mem.write(a, b)

/*-------------------------------+
 | $0000 | 2KiB internal RAM     |
 +-------+-----------------------+
 | $0800 | Mirrors of RAM        |
 |       |                       |
 |       |                       |
 +-------+-----------------------+
 | $2000 | PPU registers         |
 +-------+-----------------------+
 | $2008 | Mirrors of PPU regs   |
 |       |                       |
 |       |                       |
 +-------+-----------------------+
 | $4000 | APU and I/O registers |
 +-------+-----------------------+
 | $4020 | Cartridge space:      |
 |       |  * PRG ROM            |
 |       |  * PRG RAM            |
 |       |  * Mapper registers   |
 |/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/|
 | $FFFA | NMI vector            |
 +-------+-----------------------+
 | $FFFC | Reset vector          |
 +-------+-----------------------+
 | $FFFE | IRQ/BRK vector        |
 +-------+-----------------------*/

typedef struct {
    BYTE ram[0x0800];
    BYTE ppu[0x0008];
    BYTE apu[0x0020];
    BYTE prg[0xBFE0];
} mem_t;

extern mem_t mem;

#endif

