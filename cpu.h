#ifndef _CPU_H
#define _CPU_H

#include "common.h"
#include "mem.h"

/* Register accessors */
#define REG_A  (cpu.a)  // Accumulator
#define REG_X  (cpu.x)  // X register
#define REG_Y  (cpu.y)  // Y register
#define REG_S  (cpu.s)  // Status flags
#define REG_PC (cpu.pc) // Program counter
#define REG_SP (cpu.sp) // Stack pointer

/* Flag bit masks */
#define FLAG_C 0x01 // Bit 0 -- Carry
#define FLAG_Z 0x02 // Bit 1 -- Zero
#define FLAG_I 0x03 // Bit 2 -- Interrupt disable
#define FLAG_D 0x08 // Bit 3 -- Decimal mode
#define FLAG_B 0x10 // Bit 4 -- Break
#define FLAG_T 0x20 // Bit 5 -- Reserved
#define FLAG_V 0x40 // Bit 6 -- Overflow
#define FLAG_S 0x80 // Bit 7 -- Sign

/* Base address of stack */
#define BASE_STACK 0x100

/* Fetch operand address and store the operand
 * NOTE: Accumulator addressing mode is handled
 * manually, as it just uses REG_A as the operand
 * and it is only used 4 times.
 * TODO: Penalty cycle calculation
 */
// Immediate -- nn
#define FETCH_IMM() \
    oper_addr = REG_PC++; \
    operand = MEM_READ(oper_addr)

// Zero Page -- [nn]
#define FETCH_ZPG() \
    oper_addr = MEM_READ(REG_PC++); \
    operand = MEM_READ(oper_addr)

// Zero Page, X -- [nn + X]
#define FETCH_ZPX() \
    oper_addr = MEM_READ(REG_PC++) + (WORD)REG_X; \
    operand = MEM_READ(oper_addr)

// Zero Page, Y -- [nn + Y]
#define FETCH_ZPY() \
    oper_addr = MEM_READ(REG_PC++) + (WORD)REG_Y; \
    operand = MEM_READ(oper_addr)

// Absolute -- [nnnn]
#define FETCH_ABS() \
    oper_addr = MEM_READ(REG_PC) \
              | (MEM_READ(REG_PC + 1) << 8); \
    REG_PC += 2; \
    operand = MEM_READ(oper_addr)

// Absolute, X -- [nnnn + X]
#define FETCH_ABX() \
    oper_addr = (MEM_READ(REG_PC) \
              | (MEM_READ(REG_PC + 1) << 8)) \
              + (WORD)REG_X; \
    REG_PC += 2; \
    operand = MEM_READ(oper_addr)

// Absolute, Y -- [nnnn + Y]
#define FETCH_ABY() \
    oper_addr = (MEM_READ(REG_PC) \
              | (MEM_READ(REG_PC + 1) << 8)) \
              + (WORD)REG_Y; \
    REG_PC += 2; \
    operand = MEM_READ(oper_addr)

// Indirect -- [[nn]]
#define FETCH_IND() \
    oper_addr = MEM_READ(MEM_READ(REG_PC++)); \
    operand = MEM_READ(oper_addr)

// (Indirect, X) -- [[nn + X]]
#define FETCH_INX() \
    oper_addr = MEM_READ(MEM_READ(REG_PC++)) \
              + (WORD)REG_X; \
    operand = MEM_READ(oper_addr)

// (Indirect), Y -- [[nn] + Y]
#define FETCH_INY() \
    oper_addr = MEM_READ(MEM_READ(REG_PC++)); \
    operand = MEM_READ(oper_addr + (WORD)REG_Y)

// Relative -- used for branch instructions
#define FETCH_REL() \
    oper_addr = MEM_READ(REG_PC++); \
    if (oper_addr & 0x80) oper_addr |= 0xFF00

/* Stack operation macros */
#define PUSH_BYTE(a) \
    MEM_WRITE(BASE_STACK + REG_SP--, (a))

#define PUSH_WORD(a) \
    MEM_WRITE(BASE_STACK + REG_SP, ((a) >> 8) & 0xFF); \
    MEM_WRITE(BASE_STACK + ((REG_SP - 1) & 0xFF), (a) & 0xFF)

#define PULL_BYTE() \
    (BYTE)(MEM_READ(BASE_STACK + ++REG_SP) & 0x00FF)

#define PULL_WORD() \
    MEM_READ(BASE_STACK + (++REG_SP)) \
    | (MEM_READ(BASE_STACK + (++REG_SP)) << 8)


/* Status flag mutators */
// Set flag
#define SET_C() REG_S |= FLAG_C
#define SET_Z() REG_S |= FLAG_Z
#define SET_I() REG_S |= FLAG_I
#define SET_D() REG_S |= FLAG_D
#define SET_B() REG_S |= FLAG_B
#define SET_V() REG_S |= FLAG_V
#define SET_S() REG_S |= FLAG_S
// Clear flag
#define CLR_C() REG_S &= (~FLAG_C)
#define CLR_Z() REG_S &= (~FLAG_Z)
#define CLR_I() REG_S &= (~FLAG_I)
#define CLR_D() REG_S &= (~FLAG_D)
#define CLR_B() REG_S &= (~FLAG_B)
#define CLR_V() REG_S &= (~FLAG_V)
#define CLR_S() REG_S &= (~FLAG_S)

/* Flag calculation */
#define CALC_C(a) if ((a) & 0xFF00) SET_C(); else CLR_C()
#define CALC_Z(a) if ((a) & 0x00FF) CLR_Z(); else SET_Z()
#define CALC_S(a) if ((a) & 0x0080) SET_S(); else CLR_S()
#define CALC_V(a) \
    ; // TODO

/* 6502 data structure */
typedef struct {
    BYTE a, x, y, s, sp; // 8-bit registers
    WORD pc;             // 16-bit registers
} cpu_t;

extern cpu_t cpu;

/* Executive functions */
void cpu_init(void);
void cpu_stop(void);
void cpu_run(int cycles);

#endif

