#include "cpu.h"

/* Helper variables */
static BYTE opcode;
static BYTE operand;
static WORD oper_addr;
static WORD result;
static int cycles_left;

/* Number of cycles for each instruction,
 * indexed by opcode
 */
static int timings[256] = {
/*    |0 |1 |2 |3 |4 |5 |6 |7 |8 |9 |A |B |C |D |E |F |*/
/* 0 */ 7, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 4, 4, 6, 6,
/* 1 */ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
/* 2 */ 6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 4, 4, 6, 6,
/* 3 */ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
/* 4 */ 6, 6, 2, 8, 3, 3, 5, 5, 3, 2, 2, 2, 3, 4, 6, 6,
/* 5 */ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
/* 6 */ 6, 6, 2, 8, 3, 3, 5, 5, 4, 2, 2, 2, 5, 4, 6, 6,
/* 7 */ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
/* 8 */ 2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
/* 9 */ 2, 6, 2, 6, 4, 4, 4, 4, 2, 5, 2, 5, 5, 5, 5, 5,
/* A */ 2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 2, 4, 4, 4, 4,
/* B */ 2, 5, 2, 5, 4, 4, 4, 4, 2, 4, 2, 4, 4, 4, 4, 4,
/* C */ 2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
/* D */ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7,
/* E */ 2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6,
/* F */ 2, 5, 2, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7
};

void cpu_init(void)
{
    REG_A = 0;
    REG_X = 0;
    REG_Y = 0;
    REG_PC = 0;
    REG_SP = 0;

    opcode = 0;
    oper_addr = 0;
    operand = 0;
    cycles_left = 0;
}

void cpu_stop(void)
{
    cycles_left = 0;
}

void cpu_run(int cycles)
{
    cycles_left = cycles;

    while (cycles_left > 0) {
        opcode = MEM_READ(REG_PC++);

        switch (opcode) {
#include "opcodes.h"
        }

        cycles_left -= timings[opcode];
    }
}

