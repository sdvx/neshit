/* This is where the current opcode is
 * decoded -- this file is included within
 * a switch construct in cpu.c.
 * (The goto's are purely for efficiency)
 *
 * TODO: Cycle penalties
 * TODO: Check operand casts
 */

/* Storage */
case 0xA9: FETCH_IMM(); goto LDA;       // LDA IMM
case 0xA5: FETCH_ZPG(); goto LDA;       // LDA ZP
case 0xB5: FETCH_ZPX(); goto LDA;       // LDA ZPX
case 0xAD: FETCH_ABS(); goto LDA;       // LDA ABS
case 0xBD: FETCH_ABX(); goto LDA;       // LDA ABSX
case 0xB9: FETCH_ABY(); goto LDA;       // LDA ABSY
case 0xA1: FETCH_INX(); goto LDA;       // LDA INDX
case 0xB1: FETCH_INY();                 // LDA INDY
LDA:                                    // Load a value into A
    REG_A = operand;
    CALC_Z(REG_A);
    CALC_S(REG_A);
    break;

case 0xA6: FETCH_ZPG(); goto LDX;       // LDX ZP
case 0xB6: FETCH_ZPY(); goto LDX;       // LDX ZPY
case 0xAE: FETCH_ABS(); goto LDX;       // LDX ABS
case 0xBE: FETCH_ABY(); goto LDX;       // LDX ABSY
case 0xA2: FETCH_IMM();                 // LDX IMM
LDX:                                    // Load a value into X
    REG_X = operand;
    CALC_Z(REG_X);
    CALC_S(REG_X);
    break;

case 0xA0: FETCH_IMM(); goto LDY;       // LDY IMM
case 0xA4: FETCH_ZPG(); goto LDY;       // LDY ZP
case 0xB4: FETCH_ZPX(); goto LDY;       // LDY ZPX
case 0xAC: FETCH_ABS(); goto LDY;       // LDY ABS
case 0xBC: FETCH_ABX();                 // LDY ABSX
LDY:                                    // Load a value into Y
    REG_Y = operand;
    CALC_Z(REG_Y);
    CALC_S(REG_Y);
    break;

case 0x85: FETCH_ZPG(); goto STA;       // STA ZP
case 0x95: FETCH_ZPX(); goto STA;       // STA ZPX
case 0x8D: FETCH_ABS(); goto STA;       // STA ABS
case 0x9D: FETCH_ABX(); goto STA;       // STA ABSX
case 0x99: FETCH_ABY(); goto STA;       // STA ABSY
case 0x81: FETCH_INX(); goto STA;       // STA INDX
case 0x91: FETCH_INY();                 // STA INDY
STA:                                    // Store operand in A
    MEM_WRITE(oper_addr, REG_A);
    break;

case 0x86: FETCH_ZPG(); goto STX;       // STX ZP
case 0x96: FETCH_ZPY(); goto STX;       // STX ZPY
case 0x8E: FETCH_ABS();                 // STX ABS
STX:                                    // Store operand in X
    MEM_WRITE(oper_addr, REG_X);
    break;

case 0x84: FETCH_ZPG(); goto STY;       // STY ZP
case 0x94: FETCH_ZPX(); goto STY;       // STY ZPX
case 0x8C: FETCH_ABS();                 // STY ABS
STY:                                    // Store operand in Y
    MEM_WRITE(oper_addr, REG_Y);
    break;

case 0xAA:                              // TAX
    REG_X = REG_A;                      // Transfer A to X
    CALC_Z(REG_X);
    CALC_S(REG_X);
    break;

case 0xA8:                              // TAY
    REG_Y = REG_A;                      // Transfer A to Y
    CALC_Z(REG_Y);
    CALC_S(REG_Y);
    break;

case 0xBA:                              // TSX
    REG_X = REG_SP;                     // Transfer SP to X
    CALC_Z(REG_X);
    CALC_S(REG_X);
    break;

case 0x8A:                              // TXA
    REG_A = REG_X;                      // Transfer X to A
    CALC_Z(REG_A);
    CALC_S(REG_A);
    break;

case 0x9A:                              // TXS
    REG_SP = REG_X;                     // Transfer X to SP
    break;

case 0x98:                              // TYA
    REG_A = REG_Y;                      // Transfer Y to A
    CALC_Z(REG_A);
    CALC_S(REG_A);
    break;

/* Arithmetic */
case 0x69: FETCH_IMM(); goto ADC;       // ADC IMM
case 0x65: FETCH_ZPG(); goto ADC;       // ADC ZP
case 0x75: FETCH_ZPX(); goto ADC;       // ADC ZPX
case 0x6D: FETCH_ABS(); goto ADC;       // ADC ABS
case 0x7D: FETCH_ABX(); goto ADC;       // ADC ABSX
case 0x79: FETCH_ABY(); goto ADC;       // ADC ABSY
case 0x61: FETCH_INX(); goto ADC;       // ADC INDX
case 0x71: FETCH_INY();                 // ADC INDY
ADC:                                    // Add with Carry
    result = REG_A + operand
           + (REG_S & FLAG_C);
    REG_A = (BYTE)(result & 0x00FF);
    CALC_C(result);
    CALC_Z(result);
    CALC_V(result);
    CALC_S(result);
#ifndef NO_DECIMAL
    if (REG_S & FLAG_D) {
        CLR_C();
        if ((REG_A & 0x0F) > 0x09)
            REG_A += 0x06;
        if ((REG_A & 0xF0) > 0x90) {
            REG_A += 0x60;
            SET_C();
        }
        cycles_left--;
    }
#endif
    break;

case 0xD6: FETCH_ZPX(); goto DEC;       // DEC ZPX
case 0xCE: FETCH_ABS(); goto DEC;       // DEC ABS
case 0xDE: FETCH_ABX();                 // DEC ABSX
DEC:                                    // Decrement operand by 1
    result = operand - 1;
    MEM_WRITE(oper_addr,
        (BYTE)(result & 0x00FF));
    CALC_Z(result);
    CALC_S(result);
    break;

case 0xCA:                              // DEX
    REG_X--;                            // Decrement X by 1
    CALC_Z(REG_X);
    CALC_S(REG_X);
    break;

case 0x88:                              // DEY
    REG_Y--;                            // Decrement Y by 1
    CALC_Z(REG_Y);
    CALC_S(REG_Y);
    break;

case 0xE6: FETCH_ZPG(); goto INC;       // INC ZP
case 0xF6: FETCH_ZPX(); goto INC;       // INC ZPX
case 0xEE: FETCH_ABS(); goto INC;       // INC ABS
case 0xFE: FETCH_ABX();                 // INC ABSX
INC:                                    // Increment operand by 1
    result = operand + 1;
    MEM_WRITE(oper_addr,
        (BYTE)(result & 0x00FF));
    CALC_Z(result);
    CALC_S(result);
    break;

case 0xE8:                              // INX
    REG_X++;                            // Increment X by 1
    CALC_Z(REG_X);
    CALC_S(REG_X);
    break;

case 0xC8:                              // INY
    REG_Y++;                            // Increment Y by 1
    CALC_Z(REG_Y);
    CALC_S(REG_Y);
    break;

case 0xE9: FETCH_IMM(); goto SBC;       // SBC IMM
case 0xE5: FETCH_ZPG(); goto SBC;       // SBC ZP
case 0xF5: FETCH_ZPX(); goto SBC;       // SBC ZPX
case 0xED: FETCH_ABS(); goto SBC;       // SBC ABS
case 0xFD: FETCH_ABX(); goto SBC;       // SBC ABSX
case 0xF9: FETCH_ABY(); goto SBC;       // SBC ABSY
case 0xE1: FETCH_INX(); goto SBC;       // SBC INDX
case 0xF1: FETCH_INY();                 // SBC INDY
SBC:                                    // Subtract with Carry
    result = REG_A + (operand ^ 0xFF)
           + (REG_S & FLAG_C);
    REG_A = (BYTE)(result & 0x00FF);
    CALC_C(result);
    CALC_Z(result);
    CALC_V(result);
    CALC_S(result);
#ifndef NO_DECIMAL
    CLR_C();
    REG_A -= 0x66;
    if ((REG_A & 0x0F) > 0x09)
        REG_A += 0x06;
    if ((REG_A & 0xF0) > 0x90) {
        REG_A += 0x60;
        SET_C();
    }
    cycles_left--;
#endif
    break;

/* Bitwise */
case 0x29: FETCH_IMM(); goto AND;       // AND IMM
case 0x25: FETCH_ZPG(); goto AND;       // AND ZP
case 0x35: FETCH_ZPX(); goto AND;       // AND ZPX
case 0x2D: FETCH_ABS(); goto AND;       // AND ABS
case 0x3D: FETCH_ABX(); goto AND;       // AND ABSX
case 0x39: FETCH_ABY(); goto AND;       // AND ABSY
case 0x21: FETCH_INX(); goto AND;       // AND INDX
case 0x31: FETCH_INY();                 // AND INDY
AND:                                    // Logical "and" A with operand
    REG_A &= operand;
    CALC_Z(REG_A);
    CALC_S(REG_A);
    break;

case 0x0A:                              // ASL ACC
    result = (WORD)REG_A << 1;
    REG_A = (BYTE)(result & 0x00FF);
    goto ASL_ACC;
case 0x06: FETCH_ZPG(); goto ASL;       // ASL ZP
case 0x16: FETCH_ZPX(); goto ASL;       // ASL ZPX
case 0x0E: FETCH_ABS(); goto ASL;       // ASL ABS
case 0x1E: FETCH_ABX();                 // ASL ABSX
ASL:                                    // Shift operand left 1 bit
    result = (WORD)operand << 1;
    MEM_WRITE(oper_addr,
        (BYTE)(result & 0x00FF));
ASL_ACC:
    CALC_C(result);
    CALC_Z(result);
    CALC_S(result);
    break;

case 0x24: FETCH_ZPG(); goto BIT;       // BIT ZP
case 0x2C: FETCH_ABS();                 // BIT ABS
BIT:                                    // Test bit #(operand) in A
    CALC_Z(REG_A & operand);
    REG_S = (REG_S & 0x3F)
          | (operand & 0xC0);
    break;

case 0x49: FETCH_IMM(); goto EOR;       // EOR IMM
case 0x45: FETCH_ZPG(); goto EOR;       // EOR ZP
case 0x55: FETCH_ZPX(); goto EOR;       // EOR ZPX
case 0x4D: FETCH_ABS(); goto EOR;       // EOR ABS
case 0x5D: FETCH_ABX(); goto EOR;       // EOR ABSX
case 0x59: FETCH_ABY(); goto EOR;       // EOR ABSY
case 0x41: FETCH_INX(); goto EOR;       // EOR INDX
case 0x51: FETCH_INY();                 // EOR INDY
EOR:                                    // Logical "xor" A with operand
    REG_A ^= operand;
    CALC_Z(REG_A);
    CALC_S(REG_A);
    break;

case 0x4A:                              // LSR ACC
    operand = REG_A;
    result = REG_A >> 1;
    REG_A = (BYTE)(result & 0x00FF);
    goto LSR_ACC;
case 0x46: FETCH_ZPG(); goto LSR;       // LSR ZP
case 0x56: FETCH_ZPX(); goto LSR;       // LSR ZPX
case 0x4E: FETCH_ABS(); goto LSR;       // LSR ABS
case 0x5E: FETCH_ABX();                 // LSR ABSX
LSR:                                    // Logical shift right
    result = operand >> 1;
    MEM_WRITE(oper_addr,
        (BYTE)(result & 0x00FF));
LSR_ACC:
    if (operand & 1) SET_C();
    else CLR_C();
    CALC_Z(result);
    CALC_S(result);
    break;

case 0x09: FETCH_IMM(); goto ORA;       // ORA IMM
case 0x05: FETCH_ZPG(); goto ORA;       // ORA ZP
case 0x15: FETCH_ZPX(); goto ORA;       // ORA ZPX
case 0x0D: FETCH_ABS(); goto ORA;       // ORA ABS
case 0x1D: FETCH_ABX(); goto ORA;       // ORA ABSX
case 0x19: FETCH_ABY(); goto ORA;       // ORA ABSY
case 0x01: FETCH_INX(); goto ORA;       // ORA INDX
case 0x11: FETCH_INY();                 // ORA INDY
ORA:                                    // Logical "or" A with operand
    REG_A |= operand;
    CALC_Z(REG_A);
    CALC_S(REG_A);
    break;

case 0x2A:                              // ROL ACC
    result = ((WORD)REG_A << 1)
           | (REG_S & FLAG_C);
    REG_A = (BYTE)(result & 0x00FF);
    goto ROL_ACC;
case 0x26: FETCH_ZPG(); goto ROL;       // ROL ZP
case 0x36: FETCH_ZPX(); goto ROL;       // ROL ZPX
case 0x2E: FETCH_ABS(); goto ROL;       // ROL ABS
case 0x3E: FETCH_ABX();                 // ROL ABSX
ROL:                                    // Rotate one bit of operand left
    result = ((WORD)operand << 1)
           | (REG_S & FLAG_C);
    MEM_WRITE(oper_addr,
        (BYTE)(result & 0x00FF));
ROL_ACC:
    CALC_C(result);
    CALC_Z(result);
    CALC_S(result);
    break;

case 0x6A:                              // ROR ACC
    operand = REG_A;
    result = (REG_A >> 1)
           | ((REG_S & FLAG_C) << 7);
    REG_A  = (BYTE)(result & 0x00FF);
    goto ROR_ACC;
case 0x66: FETCH_ZPG(); goto ROR;       // ROR ZP
case 0x76: FETCH_ZPX(); goto ROR;       // ROR ZPX
case 0x6E: FETCH_ABS(); goto ROR;       // ROR ABS
case 0x7E: FETCH_ABX();                 // ROR ABSX
ROR:                                    // Rotate one bit of operand right
    result = (operand >> 1)
           | ((REG_S & FLAG_C) << 7);
    MEM_WRITE(oper_addr,
        (BYTE)(operand & 0x00FF));
ROR_ACC:
    if (operand & 1) SET_C();
    else CLR_C();
    CALC_Z(result);
    CALC_S(result);
    break;

/* Branch */
case 0x90:                              // BCC
    FETCH_REL();                        // Branch if Carry cleared
    if (!(REG_S & FLAG_C))
        goto BRANCH;
    break;

case 0xB0:                              // BCS
    FETCH_REL();                        // Branch if Carry set
    if ((REG_S & FLAG_C) == FLAG_C)
        goto BRANCH;
    break;

case 0xD0:                              // BNE
    FETCH_REL();                        // Branch if Zero cleared
    if (!(REG_S & FLAG_Z))
        goto BRANCH;
    break;

case 0xF0:                              // BEQ
    FETCH_REL();                        // Branch if Zero set
    if ((REG_S & FLAG_Z) == FLAG_Z)
        goto BRANCH;
    break;

case 0x10:                              // BPL
    FETCH_REL();                        // Branch if Sign cleared
    if (!(REG_S & FLAG_S))
        goto BRANCH;
    break;

case 0x30:                              // BMI
    FETCH_REL();                        // Branch if Sign set
    if ((REG_S & FLAG_S) == FLAG_S)
        goto BRANCH;
    break;

case 0x50:                              // BVC
    FETCH_REL();                        // Branch if Overflow cleared
    if (!(REG_S & FLAG_V))
        goto BRANCH;
    break;

case 0x70:                              // BVS
    FETCH_REL();                        // Branch if Overflow set
    if ((REG_S & FLAG_V) == FLAG_V)
        goto BRANCH;
    break;

BRANCH:
    result = REG_PC;
    REG_PC += oper_addr;
    if ((result & 0xFF00) !=
        (REG_PC & 0xFF00))
        cycles_left -= 2;
    else cycles_left--;
    break;

/* Jump */
case 0x6C: FETCH_IND(); goto JMP;       // JMP IND
case 0x4C: FETCH_ABS();                 // JMP ABS
JMP:
    REG_PC = oper_addr;                 // Jump to (operand)
    break;

case 0x20:                              // JSR
    PUSH_WORD(REG_PC - 1);              // Jump, save return address
    REG_PC = oper_addr;
    break;

case 0x40:                              // RTI
    REG_S = PULL_BYTE();                // Return from interrupt
    REG_PC = PULL_WORD();
    break;

case 0x60:                              // RTS
    REG_PC = PULL_WORD() + 1;           // Return from subroutine

/* Registers */
case 0x18:                              // CLC
    CLR_C();                            // Clear C flag
    break;

case 0xD8:                              // CLD
    CLR_D();                            // Clear D flag
    break;

case 0x58:                              // CLI
    CLR_I();                            // Clear I flag
    break;

case 0xB8:                              // CLV
    CLR_V();                            // Clear V flag
    break;

case 0xC9: FETCH_IMM(); goto CMP;       // CMP IMM
case 0xC5: FETCH_ZPG(); goto CMP;       // CMP ZP
case 0xD5: FETCH_ZPX(); goto CMP;       // CMP ZPX
case 0xCD: FETCH_ABS(); goto CMP;       // CMP ABS
case 0xDD: FETCH_ABX(); goto CMP;       // CMP ABSX
case 0xD9: FETCH_ABY(); goto CMP;       // CMP ABSY
case 0xC1: FETCH_INX(); goto CMP;       // CMP INDX
case 0xD1: FETCH_INY();                 // CMP INDY
CMP:                                    // Compare A and operand
    if (REG_A >=
       (BYTE)(operand & 0x00FF))
        SET_C();
    else CLR_C();
    if (REG_A ==
       (BYTE)(operand & 0x00FF))
        SET_Z();
    else CLR_Z();
    CALC_S((WORD)REG_A - operand);
    break;

case 0xE0: FETCH_IMM(); goto CPX;       // CPX IMM
case 0xE4: FETCH_ZPG(); goto CPX;       // CPX ZP
case 0xEC: FETCH_ABS();                 // CPX ABS
CPX:                                    // Compare X and operand
    if (REG_X >=
       (BYTE)(operand & 0x00FF))
        SET_C();
    else CLR_C();
    if (REG_X ==
       (BYTE)(operand & 0x00FF))
        SET_Z();
    else CLR_Z();
    CALC_S((WORD)REG_X - operand);
    break;

case 0xC0: FETCH_IMM(); goto CPY;       // CPY IMM
case 0xC4: FETCH_ZPG(); goto CPY;       // CPY ZP
case 0xCC: FETCH_ABS();                 // CPY ABS
CPY:                                    // Compare Y and operand
    if (REG_Y >=
       (BYTE)(operand & 0x00FF))
        SET_C();
    else CLR_C();
    if (REG_Y ==
       (BYTE)(operand & 0x00FF))
        SET_Z();
    else CLR_Z();
    CALC_S((WORD)REG_Y - operand);
    break;

case 0x38:                              // SEC
    SET_C();                            // Set C flag
    break;

case 0xF8:                              // SED
    SET_D();                            // Set D flag
    break;

case 0x78:                              // SEI
    SET_I();                            // Set I flag
    break;

/* Stack */
case 0x48:                              // PHA
    PUSH_BYTE(REG_A);                   // Push A onto stack
    break;

case 0x08:                              // PHP
    PUSH_BYTE(REG_S | FLAG_B);          // Push S onto stack
    break;

case 0x68:                              // PLA
    REG_A = PULL_BYTE();                // Pull A off stack
    CALC_Z(REG_A);
    CALC_S(REG_A);
    break;

case 0x28:                              // PLP
    REG_S = PULL_BYTE() | FLAG_T;       // Pull S off stack
    break;

/* System */
case 0x00:                              // BRK
    REG_PC++;                           // Force break
    PUSH_WORD(REG_PC);
    PUSH_BYTE(REG_S | FLAG_B);
    SET_I();
    REG_PC = MEM_READ(0xFFFE)
           | (MEM_READ(0xFFFF) << 8);
    break;

case 0xEA:                              // NOP
    break;                              // No operation

