//
// Created by jglrxavpok on 08/06/2015.
//

#ifndef CES_CPU_H
#define CES_CPU_H

#include <vector>
#include "ram.h"

struct CPU {
    uint8_t acc;
    uint8_t indX;
    uint8_t indY;
    uint8_t sp;
    uint16_t pc;

    // Following is the processor status registry, split by each of its components

    /**
     * The carry flag. Set to 1 if the result of the last operation is greater than 255. (Bit index 0 in registry)
     */
    bool carry;
    /**
     * The zero flag. Set to 1 if the result of the last operation is 0. (Bit index 1 in registry)
     */
    bool zero;
    /**
     * Prevents the cpu to respond to interrupt instructions. (Bit index 2 in registry)
     */
    bool dsblintr;
    /**
     * Enables decimal mode if set to 1. (Bit index 3 in registry)
     */
    bool decimal;
    /**
     * Set to 1 when a break instruction has been executed. (Bit index 4 in registry)
     */
    bool breakcmd;

    /**
     * Set to 1 if last result was not expected (i.e. a negative number instead of a positive one) because of the sign bit. (Bit index 6 in registry)
     */
    bool overflow;
    /**
     * Set to 1 if last result was negative. (Bit index 7 in registry)
     */
    bool neg;

    /**
     * Page boundary
     */
    bool pb;
    uint8_t* stack;
};

CPU* cpu_init(void);
void cpu_reset(CPU* cpu, RAM* ram);

void do_cycle(CPU* cpu, RAM* ram);

void jmp(CPU* cpu, uint16_t address);

uint16_t imm(CPU* cpu, RAM* ram);

uint16_t zpage(CPU* cpu, RAM* ram, uint8_t registry = 0);

uint16_t abs(CPU* cpu, RAM* ram);
uint16_t abs(CPU* cpu, RAM* ram, uint8_t registry);

uint16_t indirect_x(CPU* cpu, RAM* ram);
uint16_t indirect_y(CPU* cpu, RAM* ram);

uint16_t rel(CPU* cpu, RAM* ram);

void exec(CPU* cpu, RAM* ram, uint8_t opcode, char** name = nullptr);

uint8_t get_status(CPU* cpu);
void set_status(CPU* cpu, uint8_t val);
//|===========================================|
//|           Here come opcodes               |
//|===========================================|
void set_zero(CPU* cpu, unsigned int val);
void set_sign(CPU* cpu, unsigned int val);
void set_overflow(CPU* cpu, bool val);
void set_carry(CPU* cpu, bool val);
void set_break(CPU* cpu, bool val);
void set_interrupt(CPU* cpu, bool val);
void set_decimal(CPU* cpu, bool val);
void push(CPU* cpu, uint8_t val);
uint8_t pop(CPU* cpu);

void opADC(CPU* cpu, RAM* ram, uint16_t src);
void opAND(CPU* cpu, RAM* ram, uint16_t srcAddr);
void opASL_A(CPU *pCPU, RAM *pRAM);
void opASL(CPU* cpu, RAM* ram, uint16_t srcAddr);
void opBIT(CPU* cpu, RAM* ram, uint16_t srcAddr);
void opCMP(CPU* cpu, RAM* ram, uint16_t srcAddr);
void opCPX(CPU* cpu, RAM* ram, uint16_t srcAddr);
void opCPY(CPU* cpu, RAM* ram, uint16_t srcAddr);
void opDEC(CPU* cpu, RAM* ram, uint16_t srcAddr);
void opDEX(CPU* cpu);
void opDEY(CPU* cpu);
void opEOR(CPU* cpu, RAM* ram, uint16_t srcAddr);
void opINC(CPU* cpu, RAM* ram, uint16_t srcAddr);
void opINX(CPU* cpu);
void opINY(CPU* cpu);
void opJMP(CPU* cpu, RAM* ram, uint16_t srcAddr);
void opJSR(CPU* cpu, RAM* ram, uint16_t srcAddr);
void opLDA(CPU* cpu, RAM* ram, uint16_t srcAddr);
void opLDX(CPU* cpu, RAM* ram, uint16_t srcAddr);
void opLDY(CPU* cpu, RAM* ram, uint16_t srcAddr);
void opLSR_A(CPU* cpu);
void opLSR(CPU* cpu, RAM* ram, uint16_t srcAddr);
void opORA(CPU* cpu, RAM* ram, uint16_t srcAddr);
void opPHA(CPU* cpu);
void opPHP(CPU* cpu);
void opPLA(CPU* cpu);
void opPLP(CPU* cpu);
void opROL_A(CPU* cpu);
void opROL(CPU* cpu, RAM* ram, uint16_t srcAddr);
void opROR_A(CPU* cpu);
void opROR(CPU* cpu, RAM* ram, uint16_t srcAddr);
void opRTI(CPU* cpu);
void opRTS(CPU* cpu);
void opSBC(CPU* cpu, RAM* ram, uint16_t srcAddr);
void opSTA(CPU* cpu, RAM* ram, uint16_t srcAddr);
void opSTX(CPU* cpu, RAM* ram, uint16_t srcAddr);
void opSTY(CPU* cpu, RAM* ram, uint16_t srcAddr);
#endif //TEST_CPU_H
