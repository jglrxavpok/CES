//
// Created by jglrxavpok on 08/06/2015.
//

#include <iostream>
#include "cpu.h"
#include "binutils.h"

using namespace std;

CPU* cpu_init(void) {
    uint8_t* stack = (uint8_t*) malloc(256 * sizeof(uint8_t));
    CPU* cpu = (CPU *) malloc(4*sizeof(uint8_t)+8*sizeof(bool)+sizeof(uint16_t)+sizeof(stack));
    cpu->stack = stack;
    return cpu;
}

void do_cycle(CPU* cpu, RAM* ram) {
    int opcode = ram_read(ram, cpu->pc);
}

void cpu_reset(CPU* cpu, RAM* ram) {
    cpu->acc = 0;
    cpu->breakcmd = false;
    cpu->carry = false;
    cpu->decimal = false;
    cpu->dsblintr = false;
    cpu->neg = false;
    cpu->indX = 0;
    cpu->indY = 0;
    cpu->pb = false;
    cpu->pc = 0;
    cpu->sp = 0;
    cpu->zero = false;
    cpu->pc = 0xFFFC-1;
    jmp(cpu, abs(cpu, ram));
}

void exec(CPU* cpu, RAM* ram, uint8_t opcode, char** name) {
    unsigned int clock_cycles = 0;
    string opname = "UNKNOWN "+to_string(opcode);
    switch(opcode) {
        case 0x69:
            // ADC Immediate
            opname = "ADC Immediate";
            opADC(cpu, ram, imm(cpu, ram));
            clock_cycles += 2;
            break;

        case 0x65:
            // ADC Zero Page
            opname = "ADC Zero page";
            opADC(cpu, ram, zpage(cpu, ram));
            clock_cycles += 3;
            break;

        case 0x75:
            // ADC Zero Page X
            opname = "ADC Zero page X";
            opADC(cpu, ram, zpage(cpu, ram, cpu->indX));
            clock_cycles += 4;
            break;

        case 0x6D:
            // ADC Absolute
            opname = "ADC Absolute";
            opADC(cpu, ram, abs(cpu, ram));
            clock_cycles += 4;
            break;

        case 0x7D:
            // ADC Absolute X
            opname = "ADC Absolute X";
            opADC(cpu, ram, abs(cpu, ram, cpu->indX));
            clock_cycles += 4;
            if(cpu->pb) {
                clock_cycles++;
            }
            break;

        case 0x79:
            // ADC Absolute Y
            opname = "ADC Absolute Y";
            opADC(cpu, ram, abs(cpu, ram, cpu->indY));
            clock_cycles += 4;
            if(cpu->pb) {
                clock_cycles++;
            }
            break;

        case 0x61:
            // ADC Indirect X
            opname = "ADC Indirect X";
            opADC(cpu, ram, indirect_x(cpu, ram));
            clock_cycles += 6;
            break;

        case 0x71:
            // ADC Indirect Y
            opname = "ADC Indirect Y";
            opADC(cpu, ram, indirect_y(cpu, ram));
            clock_cycles += 5;
            if(cpu->pb) {
                clock_cycles++;
            }
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x29: // AND Immediate
            opname = "AND Immediate";
            opAND(cpu, ram, imm(cpu, ram));
            clock_cycles += 2;
            break;

        case 0x25: // AND Zero page
            opname = "AND Zero page";
            opAND(cpu, ram, zpage(cpu, ram));
            clock_cycles += 3;
            break;

        case 0x35: // AND Zero page X
            opname = "AND Zero page X";
            opAND(cpu, ram, zpage(cpu, ram, cpu->indX));
            clock_cycles += 4;
            break;

        case 0x2D: // AND Absolute
            opname = "AND Absolute";
            opAND(cpu, ram, abs(cpu, ram));
            clock_cycles += 4;
            break;

        case 0x3D: // AND Absolute X
            opname = "AND Absolute X";
            opAND(cpu, ram, abs(cpu, ram, cpu->indX));
            clock_cycles += 4;
            if(cpu->pb) {
                clock_cycles++;
            }
            break;

        case 0x39: // AND Absolute Y
            opname = "AND Absolute Y";
            opAND(cpu, ram, abs(cpu, ram, cpu->indY));
            clock_cycles += 4;
            if(cpu->pb) {
                clock_cycles++;
            }
            break;

        case 0x21: // AND Indirect X
            opname = "AND Indirect X";
            opAND(cpu, ram, indirect_x(cpu, ram));
            clock_cycles += 6;
            break;

        case 0x31: // AND Indirect Y
            opname = "AND Indirect Y";
            opAND(cpu, ram, indirect_y(cpu, ram));
            clock_cycles += 5;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x0A: // ASL A
            opname = "ASL A";
            opASL_A(cpu, ram);
            clock_cycles += 2;
            break;

        case 0x06: // ASL Zero page
            opname = "ASL Zero page";
            opASL(cpu, ram, zpage(cpu, ram));
            clock_cycles += 5;
            break;

        case 0x16: // ASL Zero page X
            opname = "ASL Zero page X";
            opASL(cpu, ram, zpage(cpu, ram, cpu->indX));
            clock_cycles += 6;
            break;

        case 0x0E: // ASL Absolute
            opname = "ASL Absolute";
            opASL(cpu, ram, abs(cpu, ram));
            clock_cycles += 6;
            break;

        case 0x1E: // ASL Absolute X
            opname = "ASL Absolute X";
            opASL(cpu, ram, abs(cpu, ram, cpu->indX));
            clock_cycles += 7;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x90: // BCC
            opname = "BCC";
            if (!cpu->carry) {
                clock_cycles += ((cpu->pc & 0xFF00) != (rel(cpu, ram) & 0xFF00) ? 2 : 1);
                jmp(cpu, rel(cpu, ram));
            }
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0xB0: // BCS
            opname = "BCS";
            if (cpu->carry) {
                clock_cycles += ((cpu->pc & 0xFF00) != (rel(cpu, ram) & 0xFF00) ? 2 : 1);
                jmp(cpu, rel(cpu, ram));
            }
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0xF0: // BEQ
            opname = "BEQ";
            if (cpu->zero) {
                clock_cycles += ((cpu->pc & 0xFF00) != (rel(cpu, ram) & 0xFF00) ? 2 : 1);
                jmp(cpu, rel(cpu, ram));
            }
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x24:
            opname = "BIT Zero page";
            clock_cycles += 3;
            opBIT(cpu, ram, zpage(cpu, ram));
            break;

        case 0x2C:
            opname = "BIT Absolute";
            opBIT(cpu, ram, abs(cpu, ram));
            clock_cycles += 4;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x30:
            opname = "BMI";
            if (cpu->neg) {
                clock_cycles += ((cpu->pc & 0xFF00) != (rel(cpu, ram) & 0xFF00) ? 2 : 1);
                jmp(cpu, rel(cpu, ram));
            }
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0xD0:
            opname = "BNE";
            if (!cpu->zero) {
                clock_cycles += ((cpu->pc & 0xFF00) != (rel(cpu, ram) & 0xFF00) ? 2 : 1);
                jmp(cpu, rel(cpu, ram));
            }
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x10:
            opname = "BPL";
            if (!cpu->neg) {
                clock_cycles += ((cpu->pc & 0xFF00) != (rel(cpu, ram) & 0xFF00) ? 2 : 1);
                jmp(cpu, rel(cpu, ram));
            }
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x00:
            opname = "BRK";
            cpu->pc++;
            push(cpu, (uint8_t) ((cpu->pc >> 8) & 0xFF));
            push(cpu, (uint8_t) (cpu->pc & 0xFF));
            set_break(cpu, true);
            push(cpu, get_status(cpu));
            set_interrupt(cpu, true);
            cpu->pc = ram_read(ram, 0xFFFE) | (ram_read(ram, 0xFFFF) << 8);
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x50:
            opname = "BVC";
            if (!cpu->overflow) {
                clock_cycles += ((cpu->pc & 0xFF00) != (rel(cpu, ram) & 0xFF00) ? 2 : 1) +1;
                jmp(cpu, rel(cpu, ram));
            }
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x70:
            opname = "BVS";
            if (cpu->overflow) {
                clock_cycles += ((cpu->pc & 0xFF00) != (rel(cpu, ram) & 0xFF00) ? 2 : 1) +1;
                jmp(cpu, rel(cpu, ram));
            }
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x18:
            opname = "CLC";
            set_carry(cpu, false);
            clock_cycles += 2;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0xD8:
            opname = "CLD";
            set_decimal(cpu, false);
            clock_cycles += 2;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x58:
            opname = "CLI";
            set_interrupt(cpu, false);
            clock_cycles += 2;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0xB8:
            opname = "CLV";
            set_overflow(cpu, false);
            clock_cycles += 2;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0xC9:
            opname = "CMP Immediate";
            opCMP(cpu, ram, imm(cpu, ram));
            clock_cycles += 2;
            break;

        case 0xC5:
            opname = "CMP Zero page";
            opCMP(cpu, ram, zpage(cpu, ram));
            clock_cycles += 3;
            break;

        case 0xD5:
            opname = "CMP Zero page X";
            opCMP(cpu, ram, zpage(cpu, ram, cpu->indX));
            clock_cycles += 4;
            break;

        case 0xCD:
            opname = "CMP Absolute";
            opCMP(cpu, ram, abs(cpu, ram));
            clock_cycles += 4;
            break;

        case 0xDD:
            opname = "CMP Absolute X";
            opCMP(cpu, ram, abs(cpu, ram, cpu->indX));
            clock_cycles += 4;
            if(cpu->pb) {
                clock_cycles++;
            }
            break;

        case 0xD9:
            opname = "CMP Absolute Y";
            opCMP(cpu, ram, abs(cpu, ram, cpu->indY));
            clock_cycles += 4;
            if(cpu->pb) {
                clock_cycles++;
            }
            break;

        case 0xC1:
            opname = "CMP Indirect X";
            opCMP(cpu, ram, indirect_x(cpu, ram));
            clock_cycles += 6;
            break;

        case 0xD1:
            opname = "CMP Indirect Y";
            opCMP(cpu, ram, indirect_y(cpu, ram));
            clock_cycles += 5;
            if(cpu->pb) {
                clock_cycles++;
            }
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0xE0:
            opname = "CPX Immediate";
            opCPX(cpu, ram, imm(cpu, ram));
            clock_cycles += 2;
            break;

        case 0xE4:
            opname = "CPX Zero page";
            opCPX(cpu, ram, zpage(cpu, ram));
            clock_cycles += 3;
            break;

        case 0xEC:
            opname = "CPX Absolute";
            opCPX(cpu, ram, abs(cpu, ram));
            clock_cycles += 4;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0xC0:
            opname = "CPY Immediate";
            opCPY(cpu, ram, imm(cpu, ram));
            clock_cycles += 2;
            break;

        case 0xC4:
            opname = "CPY Zero page";
            opCPY(cpu, ram, zpage(cpu, ram));
            clock_cycles += 3;
            break;

        case 0xCC:
            opname = "CPY Absolute";
            opCPY(cpu, ram, abs(cpu, ram));
            clock_cycles += 4;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0xC6:
            opname = "DEC Zero page";
            opDEC(cpu, ram, zpage(cpu, ram));
            clock_cycles += 5;
            break;

        case 0xD6:
            opname = "DEC Zero page X";
            opDEC(cpu, ram, zpage(cpu, ram, cpu->indX));
            clock_cycles += 6;
            break;

        case 0xCE:
            opname = "DEC Absolute";
            opDEC(cpu, ram, abs(cpu, ram));
            clock_cycles += 6;
            break;

        case 0xDE:
            opname = "DEC Absolute X";
            opDEC(cpu, ram, abs(cpu, ram, cpu->indX));
            clock_cycles += 7;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0xCA:
            opname = "DEX";
            opDEX(cpu);
            clock_cycles += 2;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x88:
            opname = "DEY";
            opDEY(cpu);
            clock_cycles += 2;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x49:
            opname = "EOR Immediate";
            opEOR(cpu, ram, imm(cpu, ram));
            clock_cycles += 2;
            break;

        case 0x45:
            opname = "EOR Zero page";
            opEOR(cpu, ram, imm(cpu, ram));
            clock_cycles += 3;
            break;

        case 0x55:
            opname = "EOR Zero page X";
            opEOR(cpu, ram, imm(cpu, ram));
            clock_cycles += 4;
            break;

        case 0x4D:
            opname = "EOR Absolute";
            opEOR(cpu, ram, abs(cpu, ram));
            clock_cycles += 4;
            break;

        case 0x5D:
            opname = "EOR Absolute X";
            opEOR(cpu, ram, abs(cpu, ram, cpu->indX));
            clock_cycles += 4;
            if(cpu->pb) {
                clock_cycles++;
            }
            break;

        case 0x59:
            opname = "EOR Absolute Y";
            opEOR(cpu, ram, abs(cpu, ram, cpu->indY));
            clock_cycles += 4;
            if(cpu->pb) {
                clock_cycles++;
            }
            break;

        case 0x41:
            opname = "EOR Indirect X";
            opEOR(cpu, ram, indirect_x(cpu, ram));
            clock_cycles += 6;
            break;

        case 0x51:
            opname = "EOR Indirect Y";
            opEOR(cpu, ram, indirect_y(cpu, ram));
            clock_cycles += 5;
            if(cpu->pb) {
                clock_cycles++;
            }
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0xE6:
            opname = "INC Zero page";
            opINC(cpu, ram, zpage(cpu, ram));
            clock_cycles += 5;
            break;

        case 0xF6:
            opname = "INC Zero page X";
            opINC(cpu, ram, zpage(cpu, ram, cpu->indX));
            clock_cycles += 5;
            break;

        case 0xEE:
            opname = "INC Absolute";
            opINC(cpu, ram, abs(cpu, ram));
            clock_cycles += 5;
            break;

        case 0xFE:
            opname = "INC Absolute X";
            opINC(cpu, ram, abs(cpu, ram, cpu->indX));
            clock_cycles += 7;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0xE8:
            opname = "INX";
            opINX(cpu);
            clock_cycles += 2;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0xC8:
            opname = "INY";
            opINY(cpu);
            clock_cycles += 2;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x4C:
            opname = "JMP Absolute";
            opJMP(cpu, ram, abs(cpu, ram));
            clock_cycles += 3;
            break;

        case 0x6C: {
            opname = "JMP Absolute";
            uint16_t location = abs(cpu, ram);

            uint16_t indirectAddr = ram_read(ram, location) + (ram_read(ram,
                                                                        (uint16_t) (((location & 0xFF) == 0xFF) ?
                                                                                    location - 0xFF : location + 1)) <<
                                                               8);
            opJMP(cpu, ram, indirectAddr);
            clock_cycles += 5;
        }
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x20:
            opname = "JSR Absolute";
            opJSR(cpu, ram, abs(cpu, ram));
            clock_cycles += 6;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0xA9:
            opname = "LDA Immediate";
            opLDA(cpu, ram, imm(cpu, ram));
            clock_cycles += 2;
            break;

        case 0xA5:
            opname = "LDA Zero page";
            opLDA(cpu, ram, zpage(cpu, ram));
            clock_cycles += 3;
            break;

        case 0xB5:
            opname = "LDA Zero page X";
            opLDA(cpu, ram, zpage(cpu, ram, cpu->indX));
            clock_cycles += 4;
            break;

        case 0xAD:
            opname = "LDA Absolute";
            opLDA(cpu, ram, abs(cpu, ram));
            clock_cycles += 4;
            break;

        case 0xBD:
            opname = "LDA Absolute X";
            opLDA(cpu, ram, abs(cpu, ram, cpu->indX));
            clock_cycles += 4;
            if(cpu->pb) {
                clock_cycles++;
            }
            break;

        case 0xB9:
            opname = "LDA Absolute Y";
            opLDA(cpu, ram, abs(cpu, ram, cpu->indY));
            clock_cycles += 4;
            if(cpu->pb) {
                clock_cycles++;
            }
            break;

        case 0xA1:
            opname = "LDA Indirect X";
            opLDA(cpu, ram, indirect_x(cpu, ram));
            clock_cycles += 6;
            break;

        case 0xB1:
            opname = "LDA Indirect Y";
            opLDA(cpu, ram, indirect_y(cpu, ram));
            clock_cycles += 5;
            if(cpu->pb) {
                clock_cycles++;
            }
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0xA2:
            opname = "LDX Immediate";
            opLDX(cpu, ram, imm(cpu, ram));
            clock_cycles += 2;
            break;

        case 0xA6:
            opname = "LDX Zero page";
            opLDX(cpu, ram, zpage(cpu, ram));
            clock_cycles += 3;
            break;

        case 0xB6:
            opname = "LDX Zero page Y";
            opLDX(cpu, ram, zpage(cpu, ram, cpu->indY));
            clock_cycles += 4;
            break;

        case 0xAE:
            opname = "LDX Absolute";
            opLDX(cpu, ram, abs(cpu, ram));
            clock_cycles += 4;
            break;

        case 0xBE:
            opname = "LDX Absolute Y";
            opLDX(cpu, ram, abs(cpu, ram, cpu->indY));
            clock_cycles += 4;
            if(cpu->pb) {
                clock_cycles++;
            }
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0xA0:
            opname = "LDY Immediate";
            opLDY(cpu, ram, imm(cpu, ram));
            clock_cycles += 2;
            break;

        case 0xA4:
            opname = "LDY Zero page";
            opLDY(cpu, ram, zpage(cpu, ram));
            clock_cycles += 3;
            break;

        case 0xB4:
            opname = "LDY Zero page X";
            opLDY(cpu, ram, zpage(cpu, ram, cpu->indX));
            clock_cycles += 4;
            break;

        case 0xAC:
            opname = "LDY Absolute";
            opLDY(cpu, ram, abs(cpu, ram));
            clock_cycles += 4;
            break;

        case 0xBC:
            opname = "LDY Absolute X";
            opLDY(cpu, ram, abs(cpu, ram, cpu->indX));
            clock_cycles += 4;
            if(cpu->pb) {
                clock_cycles++;
            }
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x4A:
            opname = "LSR A";
            opLSR_A(cpu);
            clock_cycles += 2;
            break;

        case 0x46:
            opname = "LSR Zero page";
            opLSR(cpu, ram, zpage(cpu, ram));
            clock_cycles += 5;
            break;

        case 0x56:
            opname = "LSR Zero page X";
            opLSR(cpu, ram, zpage(cpu, ram, cpu->indX));
            clock_cycles += 6;
            break;

        case 0x4E:
            opname = "LSR Absolute";
            opLSR(cpu, ram, abs(cpu, ram));
            clock_cycles += 6;
            break;

        case 0x5E:
            opname = "LSR Absolute X";
            opLSR(cpu, ram, abs(cpu, ram, cpu->indX));
            clock_cycles += 7;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0xEA:
            opname = "NOP";
            // NOP
            clock_cycles += 2;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x09:
            opname = "ORA Immediate";
            opORA(cpu, ram, imm(cpu, ram));
            clock_cycles += 2;
            break;

        case 0x05:
            opname = "ORA Zero page";
            opORA(cpu, ram, zpage(cpu, ram));
            clock_cycles += 3;
            break;

        case 0x15:
            opname = "ORA Zero page X";
            opORA(cpu, ram, zpage(cpu, ram, cpu->indX));
            clock_cycles += 4;
            break;

        case 0x0D:
            opname = "ORA Absolute";
            opORA(cpu, ram, abs(cpu, ram));
            clock_cycles += 4;
            break;

        case 0x1D:
            opname = "ORA Absolute X";
            opORA(cpu, ram, abs(cpu, ram, cpu->indX));
            clock_cycles += 4;
            if(cpu->pb) {
                clock_cycles++;
            }
            break;

        case 0x19:
            opname = "ORA Absolute Y";
            opORA(cpu, ram, abs(cpu, ram, cpu->indY));
            clock_cycles += 4;
            if(cpu->pb) {
                clock_cycles++;
            }
            break;

        case 0x01:
            opname = "ORA Indirect X";
            opORA(cpu, ram, indirect_x(cpu, ram));
            clock_cycles += 6;
            break;

        case 0x11:
            opname = "ORA Indirect Y";
            opORA(cpu, ram, indirect_y(cpu, ram));
            clock_cycles += 5;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x48:
            opname = "PHA";
            opPHA(cpu);
            clock_cycles += 3;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x08:
            opname = "PHP";
            opPHP(cpu);
            clock_cycles += 3;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x68:
            opname = "PLA";
            opPHP(cpu);
            clock_cycles += 4;
            break;


        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x28:
            opname = "PLP";
            opPLP(cpu);
            clock_cycles += 4;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x2A:
            opname = "ROL A";
            opROL_A(cpu);
            clock_cycles += 2;
            break;

        case 0x26:
            opname = "ROL Zero page";
            opROL(cpu, ram, zpage(cpu, ram));
            clock_cycles += 5;
            break;

        case 0x36:
            opname = "ROL Zero page X";
            opROL(cpu, ram, zpage(cpu, ram, cpu->indX));
            clock_cycles += 6;
            break;

        case 0x2E:
            opname = "ROL Absolute";
            opROL(cpu, ram, abs(cpu, ram));
            clock_cycles += 6;
            break;

        case 0x3E:
            opname = "ROL Absolute X";
            opROL(cpu, ram, abs(cpu, ram, cpu->indX));
            clock_cycles += 7;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x6A:
            opname = "ROR A";
            opROR_A(cpu);
            clock_cycles += 2;
            break;

        case 0x66:
            opname = "ROR Zero page";
            opROR(cpu, ram, zpage(cpu, ram));
            clock_cycles += 5;
            break;

        case 0x76:
            opname = "ROR Zero page X";
            opROR(cpu, ram, zpage(cpu, ram, cpu->indX));
            clock_cycles += 6;
            break;

        case 0x6E:
            opname = "ROR Absolute";
            opROR(cpu, ram, abs(cpu, ram));
            clock_cycles += 6;
            break;

        case 0x7E:
            opname = "ROR Absolute X";
            opROR(cpu, ram, abs(cpu, ram, cpu->indX));
            clock_cycles += 7;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x40:
            opname = "RTI";
            opRTI(cpu);
            clock_cycles += 6;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x60:
            opname = "RTS";
            opRTS(cpu);
            clock_cycles += 6;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0xE9:
            opname = "SBC Immediate";
            opSBC(cpu, ram, imm(cpu, ram));
            clock_cycles += 2;
            break;

        case 0xE5:
            opname = "SBC Zero page";
            opSBC(cpu, ram, zpage(cpu, ram));
            clock_cycles += 3;
            break;

        case 0xF5:
            opname = "SBC Zero page X";
            opSBC(cpu, ram, zpage(cpu, ram, cpu->indX));
            clock_cycles += 4;
            break;

        case 0xED:
            opname = "SBC Absolute";
            opSBC(cpu, ram, abs(cpu, ram));
            clock_cycles += 4;
            break;

        case 0xFD:
            opname = "SBC Absolute X";
            opSBC(cpu, ram, abs(cpu, ram, cpu->indY));
            clock_cycles += 4;
            if(cpu->pb) {
                clock_cycles++;
            }
            break;

        case 0xF9:
            opname = "SBC Absolute Y";
            opSBC(cpu, ram, abs(cpu, ram, cpu->indY));
            clock_cycles += 4;
            if(cpu->pb) {
                clock_cycles++;
            }
            break;

        case 0xE1:
            opname = "SBC Indirect X";
            opSBC(cpu, ram, indirect_x(cpu, ram));
            clock_cycles += 6;
            break;

        case 0xF1:
            opname = "SBC Indirect Y";
            opSBC(cpu, ram, indirect_y(cpu, ram));
            clock_cycles += 5;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x38:
            opname = "SEC";
            set_carry(cpu, true);
            clock_cycles += 2;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0xF8:
            opname = "SED";
            set_decimal(cpu, true);
            clock_cycles += 2;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x78:
            opname = "SEI";
            set_interrupt(cpu, true);
            clock_cycles += 2;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x85:
            opname = "STA Zero page";
            opSTA(cpu, ram, zpage(cpu, ram));
            clock_cycles += 3;
            break;

        case 0x95:
            opname = "STA Zero page X";
            opSTA(cpu, ram, zpage(cpu, ram, cpu->indX));
            clock_cycles += 3;
            break;

        case 0x8D:
            opname = "STA Absolute";
            opSTA(cpu, ram, abs(cpu, ram));
            clock_cycles += 4;
            break;

        case 0x9D:
            opname = "STA Absolute X";
            opSTA(cpu, ram, abs(cpu, ram, cpu->indX));
            clock_cycles += 5;
            break;

        case 0x99:
            opname = "STA Absolute Y";
            opSTA(cpu, ram, abs(cpu, ram, cpu->indY));
            clock_cycles += 5;
            break;

        case 0x81:
            opname = "STA Indirect X";
            opSTA(cpu, ram, indirect_x(cpu, ram));
            clock_cycles += 6;
            break;

        case 0x91:
            opname = "STA Indirect Y";
            opSTA(cpu, ram, indirect_y(cpu, ram));
            clock_cycles += 6;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x86:
            opname = "STX Zero page";
            opSTX(cpu, ram, zpage(cpu, ram));
            clock_cycles += 3;
            break;

        case 0x96:
            opname = "STX Zero page Y";
            opSTX(cpu, ram, zpage(cpu, ram, cpu->indY));
            clock_cycles += 4;
            break;

        case 0x8E:
            opname = "STX Absolute";
            opSTX(cpu, ram, abs(cpu, ram));
            clock_cycles += 4;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x84:
            opname = "STY Zero page";
            opSTY(cpu, ram, zpage(cpu, ram));
            clock_cycles += 3;
            break;

        case 0x94:
            opname = "STY Zero page Y";
            opSTY(cpu, ram, zpage(cpu, ram, cpu->indX));
            clock_cycles += 4;
            break;

        case 0x8C:
            opname = "STY Absolute";
            opSTY(cpu, ram, abs(cpu, ram));
            clock_cycles += 4;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0xAA:
            opname = "TAX";
            cpu->indX = cpu->acc;
            clock_cycles += 2;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0xA8:
            opname = "TAY";
            cpu->indY = cpu->acc;
            clock_cycles += 2;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0xBA:
            opname = "TSX";
            cpu->indX = cpu->sp;
            clock_cycles += 2;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x8A:
            opname = "TXA";
            cpu->acc = cpu->indX;
            clock_cycles += 2;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x89:
            opname = "TXS";
            cpu->sp = cpu->indX;
            clock_cycles += 2;
            break;

        //============================== OP SEPARATOR, DON'T MIND ME ==============================//

        case 0x98:
            opname = "TYA";
            cpu->acc = cpu->indY;
            clock_cycles += 2;
            break;

        // START OF ILLEGAL OPCODES

        default:
            break;
    }

    *name = (char *) opname.c_str();
}

void opSTY(CPU* cpu, RAM* ram, uint16_t srcAddr) {
    ram_write(ram, srcAddr, cpu->indY);
}

void opSTX(CPU* cpu, RAM* ram, uint16_t srcAddr) {
    ram_write(ram, srcAddr, cpu->indX);
}

void opSTA(CPU* cpu, RAM* ram, uint16_t srcAddr) {
    ram_write(ram, srcAddr, cpu->acc);
}

void opSBC(CPU* cpu, RAM* ram, uint16_t srcAddr) {
    uint8_t src = ram_read(ram, srcAddr);
    unsigned int tmp = (unsigned int) (cpu->acc - src - (cpu->carry ? 0 : 1));
    set_sign(cpu, tmp);
    set_zero(cpu, tmp & 0xFF);
    set_overflow(cpu, ((cpu->acc ^ tmp) & 0x80) && ((cpu->acc ^ src) & 0x80));
    if(cpu->decimal) {
        if ( ((cpu->acc & 0xf) - (cpu->acc ? 0 : 1)) < (src & 0xf)) /* EP */ tmp -= 6;
        if (tmp > 0x99) tmp -= 0x60;
    }
    set_carry(cpu, tmp < 0x100);
    cpu->acc = (uint8_t) (tmp & 0xFF);
}

void opRTS(CPU* cpu) {
    uint8_t src = pop(cpu);
    src |= pop(cpu) << 8;
    src++;
    cpu->pc = src;
}

void opRTI(CPU* cpu) {
    uint8_t src = pop(cpu);
    set_status(cpu, src);
    src = pop(cpu);
    src |= pop(cpu) << 8;
    cpu->pc = src;
}

void opROR(CPU* cpu, RAM* ram, uint16_t srcAddr) {
    uint8_t src = ram_read(ram, srcAddr);
    if(cpu->carry) {
        src |= 0x100;
    }
    set_carry(cpu, (bool) (src & 0x01));
    src >>= 1;
    set_sign(cpu, src);
    set_zero(cpu, src);
    ram_write(ram, srcAddr, src);
}

void opROR_A(CPU* cpu) {
    uint8_t src = cpu->acc;
    if(cpu->carry) {
        src |= 0x100;
    }
    set_carry(cpu, (bool) (src & 0x01));
    src >>= 1;
    set_sign(cpu, src);
    set_zero(cpu, src);
    cpu->acc = src;
}

void opROL(CPU* cpu, RAM* ram, uint16_t srcAddr) {
    uint8_t src = ram_read(ram, srcAddr);
    src <<= 1;
    if(cpu->carry) {
        src |= 0x1;
    }
    set_carry(cpu, src > 0xFF);
    src &= 0xFF;
    set_sign(cpu, src);
    set_zero(cpu, src);
    ram_write(ram, srcAddr, src);
}

void opROL_A(CPU* cpu) {
    uint8_t src = cpu->acc;
    src <<= 1;
    if(cpu->carry) {
        src |= 0x1;
    }
    set_carry(cpu, src > 0xFF);
    src &= 0xFF;
    set_sign(cpu, src);
    set_zero(cpu, src);
    cpu->acc = src;
}

void opPLP(CPU* cpu) {
    uint8_t src = pop(cpu);
    set_status(cpu, src);
}

void opPLA(CPU* cpu) {
    uint8_t src = pop(cpu);
    set_sign(cpu, src);
    set_zero(cpu, src);
    cpu->acc = src;
}

void opPHP(CPU* cpu) {
    push(cpu, get_status(cpu));
}

void opPHA(CPU* cpu) {
    push(cpu, cpu->acc);
}

void opORA(CPU* cpu, RAM* ram, uint16_t srcAddr) {
    uint8_t src = ram_read(ram, srcAddr);
    src |= cpu->acc;
    set_sign(cpu, src);
    set_zero(cpu, src);
    cpu->acc = src;
}

void opLSR(CPU* cpu, RAM* ram, uint16_t srcAddr) {
    uint8_t src = ram_read(ram, srcAddr);
    set_carry(cpu, (bool) (src & 0x01));
    src >>= 1;
    set_sign(cpu, src);
    set_zero(cpu, src);
    ram_write(ram, srcAddr, src);
}

void opLSR_A(CPU* cpu) {
    uint8_t src = cpu->acc;
    set_carry(cpu, (bool) (src & 0x01));
    src >>= 1;
    set_sign(cpu, src);
    set_zero(cpu, src);
    cpu->acc = src;
}

void opLDY(CPU* cpu, RAM* ram, uint16_t srcAddr) {
    uint8_t src = ram_read(ram, srcAddr);
    set_zero(cpu, src);
    set_sign(cpu, src);
    cpu->indY = src;
}

void opLDX(CPU* cpu, RAM* ram, uint16_t srcAddr) {
    uint8_t src = ram_read(ram, srcAddr);
    set_zero(cpu, src);
    set_sign(cpu, src);
    cpu->indX = src;
}

void opLDA(CPU* cpu, RAM* ram, uint16_t srcAddr) {
    uint8_t src = ram_read(ram, srcAddr);
    set_zero(cpu, src);
    set_sign(cpu, src);
    cpu->acc = src;
}

void opJSR(CPU* cpu, RAM* ram, uint16_t srcAddr) {
    cpu->pc--;
    push(cpu, (uint8_t) ((cpu->pc >> 8) & 0xFF));
    push(cpu, (uint8_t) (cpu->pc & 0xFF));
    cpu->pc = ram_read(ram, srcAddr);
}

void opJMP(CPU* cpu, RAM* ram, uint16_t srcAddr) {
    cpu->pc = ram_read(ram, srcAddr);
}

void opINY(CPU* cpu) {
    uint8_t src = cpu->indY;
    src = (uint8_t) ((src + 1) & 0xFF);
    set_sign(cpu, src);
    set_zero(cpu, src);
    cpu->indY = src;
}

void opINX(CPU* cpu) {
    uint8_t src = cpu->indX;
    src = (uint8_t) ((src + 1) & 0xFF);
    set_sign(cpu, src);
    set_zero(cpu, src);
    cpu->indX = src;
}

void opINC(CPU* cpu, RAM* ram, uint16_t srcAddr) {
    uint8_t src = ram_read(ram, srcAddr);
    src = (uint8_t) ((src + 1) & 0xFF);
    set_sign(cpu, src);
    set_zero(cpu, src);
    ram_write(ram, srcAddr, src);
}

void opEOR(CPU* cpu, RAM* ram, uint16_t srcAddr) {
    uint8_t src = ram_read(ram, srcAddr);
    src ^= cpu->acc;
    set_sign(cpu, src);
    set_zero(cpu, src);
    cpu->acc = src;
}

void opDEY(CPU* cpu) {
    uint8_t src = cpu->indY;
    src = (uint8_t) ((src-1) & 0xFF);
    set_sign(cpu, src);
    set_zero(cpu, src);
    cpu->indY = src;
}

void opDEX(CPU* cpu) {
    uint8_t src = cpu->indX;
    src = (uint8_t) ((src-1) & 0xFF);
    set_sign(cpu, src);
    set_zero(cpu, src);
    cpu->indX = src;
}

void opDEC(CPU* cpu, RAM* ram, uint16_t srcAddr) {
    uint8_t src = ram_read(ram, srcAddr);
    src = (uint8_t) ((src-1) & 0xFF);
    set_sign(cpu, src);
    set_zero(cpu, src);
    ram_write(ram, srcAddr, src);
}

void opCPY(CPU* cpu, RAM* ram, uint16_t srcAddr) {
    uint8_t src = ram_read(ram, srcAddr);
    src = cpu->indY-src;
    set_carry(cpu, src < 0x100);
    set_sign(cpu, src);
    src &= 0xff;
    set_zero(cpu, src);
}

void opCPX(CPU* cpu, RAM* ram, uint16_t srcAddr) {
    uint8_t src = ram_read(ram, srcAddr);
    src = cpu->indX-src;
    set_carry(cpu, src < 0x100);
    set_sign(cpu, src);
    src &= 0xff;
    set_zero(cpu, src);
}

void opCMP(CPU* cpu, RAM* ram, uint16_t srcAddr) {
    uint8_t src = ram_read(ram, srcAddr);
    src = cpu->acc-src;
    set_carry(cpu, src < 0x100);
    set_sign(cpu, src);
    src &= 0xff;
    set_zero(cpu, src);
}

void opBIT(CPU* cpu, RAM* ram, uint16_t srcAddr) {
    uint8_t src = ram_read(ram, srcAddr);
    set_sign(cpu, src);
    set_overflow(cpu, (bool) (0x40 & src));	/* Copy bit 6 to OVERFLOW flag. */
    set_zero(cpu, src & cpu->acc);
}

void opASL(CPU* cpu, RAM* ram, uint16_t srcAddr) {
    uint8_t src = ram_read(ram, srcAddr);
    set_carry(cpu, (bool) (src & 0x80));
    src <<= 1;
    src &= 0xFF;
    set_sign(cpu, src);
    set_zero(cpu, src);
    ram_write(ram, srcAddr, src);
}

void opASL_A(CPU* cpu, RAM* ram) {
    set_carry(cpu, (bool) (cpu->acc & 0x80));
    cpu->acc <<= 1;
    cpu->acc &= 0xFF;
    set_sign(cpu, cpu->acc);
    set_zero(cpu, cpu->acc);
}

void opAND(CPU* cpu, RAM* ram, uint16_t srcAddr) {
    uint8_t src = ram_read(ram, srcAddr);
    src &= cpu->acc;
    set_sign(cpu, src);
    set_zero(cpu, src);
    cpu->acc = src;
}

void opADC(CPU* cpu, RAM* ram, uint16_t srcAddr) {
    uint8_t src = ram_read(ram, srcAddr);
    unsigned int tmp = src + cpu->acc + (cpu->carry ? 1 : 0);
    set_zero(cpu, tmp & 0xFF);
    if(cpu->decimal) {
        if((cpu->acc & 0xF) + (src & 0xF) + (cpu->carry ? 1 : 0) > 9) {
            tmp += 6;
            set_sign(cpu, tmp);
            set_overflow(cpu, !((cpu->acc ^ src) & 0x80) && ((cpu->acc ^ tmp) & 0x80));
            if (tmp > 0x99) tmp += 96;
            set_carry(cpu, tmp > 0x99);
        }
    } else {
        set_sign(cpu, tmp);
        set_overflow(cpu, !((cpu->acc ^ src) & 0x80) && ((cpu->acc ^ tmp) & 0x80));
        set_carry(cpu, tmp > 0xFF);
    }
    cpu->acc = (uint8_t) tmp;
}

void push(CPU* cpu, uint8_t val) {
    cpu->stack[cpu->sp] = val;
}

uint8_t pop(CPU* cpu) {
    uint8_t val = cpu->stack[cpu->sp-1];
    cpu->sp--;
    return val;
}

void set_status(CPU* cpu, uint8_t val) {
    cpu->carry = get_flag(&val, 0);
    cpu->zero = get_flag(&val, 1);
    cpu->dsblintr = get_flag(&val, 2);
    cpu->decimal = get_flag(&val, 3);
    cpu->breakcmd = get_flag(&val, 4);
    cpu->overflow = get_flag(&val, 6);
    cpu->neg = get_flag(&val, 7);
}

uint8_t get_status(CPU* cpu) {
    uint8_t status = (uint8_t) (cpu->carry << 0);
    status |= cpu->zero << 1;
    status |= cpu->dsblintr << 2;
    status |= cpu->decimal << 3;
    status |= cpu->breakcmd << 4;
    status |= cpu->overflow << 6;
    status |= cpu->neg << 7;
    return status;
}

void set_decimal(CPU* cpu, bool val) {
    cpu->decimal = val;
}

void set_break(CPU* cpu, bool val) {
    cpu->breakcmd = val;
}

void set_interrupt(CPU* cpu, bool val) {
    cpu->dsblintr = val;
}

void set_carry(CPU* cpu, bool val) {
    cpu->carry = val;
}

void set_zero(CPU* cpu, unsigned int val) {
    cpu->zero = val == 0;
}

void set_overflow(CPU* cpu, bool val) {
    cpu->overflow = val;
}

void set_sign(CPU* cpu, unsigned int val) {
    cpu->neg = get_flag(&val, 7);
}

void jmp(CPU* cpu, uint16_t address) {
    cpu->pc = address;
}

// Start of addressing modes

uint16_t imm(CPU* cpu, RAM* ram) {
    return cpu->pc++;
}

uint16_t zpage(CPU* cpu, RAM* ram, uint8_t registry) {
    cpu->pc++;
    uint8_t addr = ram_read(ram, cpu->pc)+registry;
    return addr;
}

uint16_t abs(CPU* cpu, RAM* ram) {
    cpu->pc++;
    uint8_t low = ram_read(ram, cpu->pc);
    cpu->pc++;
    uint8_t high = ram_read(ram, cpu->pc);
    return (high << 8) | low;
}

uint16_t abs(CPU* cpu, RAM* ram, uint8_t registry) {
    uint16_t absAddr = abs(cpu, ram);
    if(absAddr + registry >> 8 != absAddr >> 8)
        cpu->pb = true;

    int readAddr = ((absAddr & 0xFF00) | ((absAddr + registry) & 0xFF)) & 0xFFFF;
    ram_read(ram, (uint16_t)readAddr);
    return absAddr + registry;
}

uint16_t rel(CPU* cpu, RAM* ram) {
    uint8_t src = ram_read(ram, (uint16_t) (cpu->pc+1));
    return cpu->pc + src;
}

uint16_t indirect_x(CPU* cpu, RAM* ram) {
    uint8_t addr = (uint8_t) zpage(cpu, ram);
    uint8_t target = (uint8_t) ((addr + cpu->indX) & 0x00FF);
    return (uint16_t) (target + (target + 1) << 8);
}

uint16_t indirect_y(CPU* cpu, RAM* ram) {
    uint16_t addr = zpage(cpu, ram) + (zpage(cpu, ram) << 8);
    return addr+cpu->indY;
}