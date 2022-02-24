#include "chip8.h" 
#include <cmath>
#include <stdint.h>

void chip8::initialize()
{
    // Clear opcode
    opcode = 0; 

    // Set PC to starting location
    PC = 0x200; 
    // Clear registers
    I = 0; 
    for (int i = 0; i < 16; i++) {
        V[i] = 0; 
    }

    // Clear stack and sp
    sp = 0; 
    for (int i = 0; i < 16; i++) {
        stack[i] = 0; 
    }

    // Load font sprites beginning at address 0x050
    for (int i = 0x050; i < 80 + 0x050; i++) {
        memory[i] = chip8_fontset[i]; 
    }

}

void chip8::runCycle()
{
    /*
    Beacuse we are fetching two bytes for each instruction, 
    the first byte is shifted left 8-bits before performing bitwise or
    */ 
    opcode = memory[PC] << 8 | memory[PC + 1]; 

    // PC is then incremented by 2
    PC += 2; 

    // decode
    uint16_t opIns0 = (opcode & 0xF000);  
    uint16_t opIns1 = (opcode & 0x000F); 
    uint8_t opIns2  = (opcode & 0x000F); 

    uint8_t opInsByte  = (opcode & 0x00FF);  
    uint8_t opInsX     = (opcode & 0x0F00); 
    uint8_t opInsY     = (opcode & 0x00F0); 
    uint16_t opInsLong = (opcode & 0x0FFF); 
    

    // decode opcode
    // execute opcode
    switch (opIns0) {
        case 0x0000: 
        switch (opIns1) {
            case(0x0000):
                // 00E0 - CLS: clear the display 
                break;  
            case(0x000E):
                // 00EE - RET: set PC to top of stack, decrement stack pointer 
                PC = stack[sp]; 
                sp--; 
                break;
        } 
        case 0x1000:
            // 1nnn - JP addr: set PC to value nnn
            PC = opInsLong; 
            break;
        case 0x2000:
            // 2nnn - CALL addr: store PC in stack, set PC to nnn 
            sp ++;
            stack[sp] = PC;
            PC = opInsLong; 
            break;
        case 0x3000:
            // 3xnn - SE Vx, byte: skip next instruction if V[x] == byte 
            if (V[opInsX] == opInsByte) PC += 2; 
            break;
        case 0x4000:
            // 4xnn - SNE Vx, byte: skip next instruction if V[x] != byte 
            if (V[opInsX] != opInsByte) PC += 2; 
            break;
        case 0x5000:
            // 5xy0 - SE Vx, Vy: skip next instruction if Vx = Vy
            if (V[opInsX] == V[opInsY]) PC != 2; 
            break;
        case 0x6000:
            // 6xnn - LD Vx: set register Vx to nn
            V[opInsX] = opInsByte;    
            break;
        case 0x7000:
            // 7xnn - ADD Vx, byte: adds nn to Vx then stores in Vx
            V[opInsX] = V[opInsX] + opInsByte; 
            break;
        case 0x8000:
        switch(opIns1) {
            case 0x0001:
                // 8xy1 - OR Vx, Vy: stores result of Vx | Vy in Vx
                V[opInsX] = V[opInsX] | V[opInsY]; 
                break;
            case 0x0002:
                // 8xy2 - AND Vx, Vy: stores result of Vx & Vy in Vx
                V[opInsX] = V[opInsX] & V[opInsY]; 
                break;
            case 0x0003:
                // 8xy3 - XOR Vx, Vy: stores result of Vx ^ Vy in Vx
                V[opInsX] = V[opInsX] ^ V[opInsY]; 
                break;
            case 0x0004:
                // 8xy4 - ADD Vx, Vy: stores result of Vx + Vy in Vx
                // if result if > 255, set VF == 1, else 0
                uint16_t val = V[opInsX] + V[opInsY];
                val > 255 ? V[15] = 1 : V[15] = 0;  
                V[opInsX] = val;
                break;
            case 0x0005:
                // 8xy5 - SUB Vx, Vy: stores result of Vx - Vy in Vx
                // if Vx > Vy, set VF = 1 else 0 
                V[opInsX] > V[opInsY] ? V[15] = 1 : V[15] = 0;
                V[opInsX] = V[opInsX] - V[opInsY]; 
                break;
            case 0x0006:
                // 8xy6 - SHR Vx {, Vy}: stores result of Vx >> 1
                // VF is set = 1 if Vx % 2 != 0
                V[opInsX] % 2 == 0 ? V[15] = 0 : V[15] = 1; 
                V[opInsX] = V[opInsX] / 2; 
                break;
            case 0x0007:
                // 8xy7 - SUB Vx, Vy: stores result of Vy - Vx in Vx
                // if Vy > Vx, set VF = 1 else 0 
                V[opInsY] > V[opInsX] ? V[15] = 1 : V[15] = 0;
                V[opInsX] = V[opInsY] - V[opInsX]; 
                break;
            case 0x000E:
                // TODO
                // 8xyE - SHL Vx {, Vy}: stores result of Vx << 1
                // VF is set = 1 if MSB == 1
                log2(V[opInsX]) == 1 ? V[15] = 1 : V[15] = 0;  
                V[opInsX] = V[opInsX] * 2; 
        }    
            break; 
        case 0x9000:
            break;
        case 0xA000:
            // Annn - LD I, addr: set index register to value nnn
            I = opInsLong; 
            break;
        case 0xB000:
            break;
        case 0xC000:
            break;
        case 0xD000:

            break;
        case 0xE000:
            break;
        case 0xF000:
            break;
    }
}