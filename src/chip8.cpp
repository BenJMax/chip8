#include "chip8.h" 
#include <cmath>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <ios>
#include <vector> 


uint8_t chip8_fontset[80] = { 
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


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

    // Set memory to zero 
    for (int i = 0; i < 4096; i++) {
        memory[i] = 0; 
    }

    // Load font sprites beginning at address 0x050
    for (int i = 0x050; i < 80 + 0x050; i++) {
        memory[i] = chip8_fontset[i]; 
    }

    drawFlag = false; 
    delay_timer = 0;
    sound_timer = 0; 

    srand(time(NULL)); 
}

void chip8::runCycle()
{
    /*
    Beacuse we are fetching two bytes for each instruction, 
    the firSt byte is shifted left 8-bits before performing bitwise or
    */ 
    opcode = memory[PC] << 8 | memory[PC + 1]; 

    // decode
    uint16_t opIns0 = (opcode & 0xF000);  
    uint16_t opIns1 = (opcode & 0x000F); 
    uint8_t opIns2  = (opcode & 0x000F); 

    uint8_t opInsByte  = (opcode & 0x00FF);  
    uint8_t opInsX     = (opcode & 0x0F00) >> 8; 
    uint8_t opInsY     = (opcode & 0x00F0) >> 4; 
    uint16_t opInsLong = (opcode & 0x0FFF); 

    // decode opcode
    // execute opcode
    switch (opIns0){
        case 0x0000: 
            switch (opIns1){
                case(0x0000): 
                    // 00E0 - CLS: clear the display 
                    for (int i = 0; i < PIXEL_H * PIXEL_W; i++) {
                        graphics[i] = 0; 
                    }
                    drawFlag = true; 
                    PC += 2; 
                    break; 
                 
                case(0x000E): 
                    // 00EE - RET: set PC to top of stack, decrement stack pointer 
                    sp--; 
                    PC = stack[sp]; 
                    PC += 2; 
                break;

                default: 
                    printf("Unknown opcode 0x%X\n", opcode); 
                break;  

            }
        break; 

        case 0x1000: 
            // 1nnn - JP addr: set PC to value nnn
            PC = opInsLong; 
        break;

        case 0x2000: 
            // 2nnn - CALL addr: store PC in stack, set PC to nnn 
            stack[sp] = PC;
            sp ++;
            PC = opInsLong; 
        break;

        case 0x3000: 
            // 3xnn - SE Vx, byte: skip next instruction if V[x] == byte 
            V[opInsX] == opInsByte ? PC += 4 : PC += 2; 
        break;
        
        case 0x4000: 
            // 4xnn - SNE Vx, byte: skip next instruction if V[x] != byte 
            V[opInsX] != opInsByte ? PC += 4 : PC += 2; 
        break;

        case 0x5000:
            // 5xy0 - SE Vx, Vy: skip next instruction if Vx = Vy
            V[opInsX] == V[opInsY] ? PC += 4 : PC += 2; 
        break;
        
        case 0x6000:
            // 6xnn - LD Vx: set register Vx to nn
            V[opInsX] = opInsByte;    
            PC += 2; 
        break;
        
        case 0x7000:
            // 7xnn - ADD Vx, byte: adds nn to Vx then stores in Vx
            V[opInsX] = V[opInsX] + opInsByte; 
            PC += 2; 
        break;

        case 0x8000: 
            switch(opIns1){
                case 0x0000:
                    // 8yx0 - LD Vx, Vy: Stores value of V[Y] in V[X]
                    V[opInsX] = V[opInsY]; 
                    PC += 2; 
                break; 

                case 0x0001:
                    // 8xy1 - OR Vx, Vy: stores result of Vx | Vy in Vx
                    V[opInsX] = V[opInsX] | V[opInsY]; 
                    PC += 2; 
                break;

                case 0x0002:
                    // 8xy2 - AND Vx, Vy: stores result of Vx & Vy in Vx
                    V[opInsX] = V[opInsX] & V[opInsY]; 
                    PC += 2; 
                break;

                case 0x0003:
                    // 8xy3 - XOR Vx, Vy: stores result of Vx ^ Vy in Vx
                    V[opInsX] = V[opInsX] ^ V[opInsY]; 
                    PC += 2; 
                break;

                case 0x0004:
                    // 8xy4 - ADD Vx, Vy: stores result of Vx + Vy in Vx
                    // if result if > 255, set VF == 1, else 0
                    V[opInsX] + V[opInsY] > 255 ? V[15] = 1 : V[15] = 0;  
                    V[opInsX] = V[opInsX] + V[opInsY];
                    PC += 2; 
                break;

                case 0x0005:
                    // 8xy5 - SUB Vx, Vy: stores result of Vx - Vy in Vx
                    // if Vx > Vy, set VF = 1 else 0 
                    V[opInsX] > V[opInsY] ? V[15] = 1 : V[15] = 0;
                    V[opInsX] = V[opInsX] - V[opInsY]; 
                    PC += 2; 
                break;

                case 0x0006:
                    // 8xy6 - SHR Vx {, Vy}: stores result of Vx >> 1
                    // VF is set = 1 if Vx % 2 != 0
                    V[opInsX] % 2 == 0 ? V[15] = 0 : V[15] = 1; 
                    V[opInsX] = V[opInsX] / 2; 
                    PC += 2; 
                break;

                case 0x0007:
                    // 8xy7 - SUB Vx, Vy: stores result of Vy - Vx in Vx
                    // if Vy > Vx, set VF = 1 else 0 
                    V[opInsY] > V[opInsX] ? V[15] = 1 : V[15] = 0;
                    V[opInsX] = V[opInsY] - V[opInsX]; 
                    PC += 2; 
                break;
                
                case 0x000E:
                    // 8xyE - SHL Vx {, Vy}: If MSB of Vx is 1, VF = 1, else 0. Vx *= 2
                    V[0xF] = V[opInsX] >> 7;
                    V[(opcode & 0x0F00) >> 8] <<= 1;
                    PC += 2;
                break; 

                default: 
                    printf("Invalid opcode 0x%X\n", opcode); 
                break; 
            }    
        break; 
        
        case 0x9000: 
            // 9xy0 - SNE Vx, Vy: if Vx != Vy, PC += 2 
            V[opInsX] != V[opInsY] ? PC += 4 : PC += 2; 
        break;

        case 0xA000:
            // Annn - LD I, addr: set index register to value nnn
            I = opInsLong; 
            PC += 2; 
        break;

        case 0xB000:
            // Bnnn - JP V0, addr: PC set to nnn + V0 
            PC = V[0x0] + opInsLong; 
        break;
            
        case 0xC000:
            // Cxkk - RND Vx, byte: generate rand number 0-255, & w/ kk, st in Vx 
            V[opInsX] = (rand() & 0xFF) & (opInsByte); 
            PC += 2; 
        break;

        case 0xD000:{
            /*
            Dxyn - DRW Vx, Vy, nibble
            Display an n-byte sprite starting at the mem location at I, set VF = collision 
            Read N bytes from memory starting at I. Bytes are displayed at Vx, Vy on screen
            */
            uint16_t height = opIns1;  
            uint16_t xCord = V[opInsX]; 
            uint16_t yCord = V[opInsY]; 
            uint16_t pixel; 

            V[0xF] = 0; 

            for (int y = 0; y < height; y++) {
                pixel = memory[I + y];  
                for (int x = 0; x < 8; x++) {
                    if ((pixel & (0x80 >> x)) != 0) {
                        if (graphics[xCord + x + ((yCord + y) * 64)] == 1) {
                            V[0xF] = 1; 
                        }
                        graphics[xCord + x + ((yCord + y) * 64)] ^= 1; 
                    }
                }
            }
             
            drawFlag = true; 
            PC += 2; 
        }
        break; 

        case 0xE000:{ 
            switch (opInsByte){
                case 0x009E: 
                // Ex9E - SKP Vx: skip instruction if key with val Vx is pressed
                    key[V[opInsX]] == 1 ? PC += 4 : PC += 2; 
                break;
            
                case 0x00A1:
                // ExA1 - SKPN Vx: skip instruction if key with val Vx ~ pressed
                    key[V[opInsX]] != 1 ? PC += 4 : PC += 2; 
                break;
            
                default:
                    printf ("Unkown opcode: Ex%X\n", opcode);
                break; 
            }
        }
        break;
        
        case 0xF000:{
            switch (opcode & 0x00FF){
                case 0x0007: 
                // Fx07 - LD Vx, DT: value of DT is placed in Vx
                    V[(opcode & 0x0F00) >> 8] = delay_timer;
                    PC += 2;
                break;
            
                case 0x000A:{ 
                // Fx0A - LD Vx, K: wait for key press. store value of key in Vx
                    bool keypress = false;
                    for(int i = 0; i< 16; ++i){
                        if (key[i] == 1) {
                            V[(opcode & 0x0F00) >> 8] = i;
                            keypress = true;
                        }
                    }
                
                    if(!keypress)
                        return; 
                    PC += 2;
                } 
                break;
            
              case 0x0015: // opcode fx15
                 delay_timer = V[(opcode & 0x0F00) >> 8];
                 PC += 2;
              break;
                  
              case 0x0018: //opcode fx18
                sound_timer = V[(opcode & 0x0F00) >> 8];
                PC += 2;
              break;
                  
              case 0x001E: //opcode fx1e
                if( I + V[(opcode & 0x0F00) >> 8] > 0xFFF){
                  V[0xF] = 1;
                }
                else {
                  V[0xF] = 0;
                }
                I = I + V[(opcode & 0x0F00) >> 8];
                PC += 2;
              break;
                
              case 0x0029: //opcode fx29
                I = V[(opcode & 0x0F00) >> 8] *5;
                PC += 2;
              break;
            
              case 0x0033: //opcode fx33
                memory[I]  = V[(opcode & 0x0F00) >> 8] / 100;
                memory[I+1] = (V[(opcode & 0x0F00) >> 8] /10) % 10;
                memory[I+2] = (V[(opcode & 0xF00) >> 8] % 100) % 10;
                PC += 2;
              break;
            
              case 0x0055:{  //opcode fx55
                unsigned char X = ((opcode & 0x0F00) >> 8);
                for (unsigned char r = 0; r <= X; r++) {
                    memory[I+r] = V[r];
                }
                
                I = I + X + 1;
                PC += 2;
              }
              break;
            
              case 0x0065:{ //opcode fx65
                unsigned char X = ((opcode & 0x0F00) >> 8);
                for (unsigned char r = 0; r <= X; r++) {
                    V[r] = memory [I+r];  
                }
                
                I = I + X + 1;
                PC += 2;
              }
              break;
            
              default:
              printf ("Unkown opcode: Fx%X\n", opcode);
                
            }
        }
        break;
    }

    //Update Timers
    if(delay_timer > 0)
        --delay_timer;
            
    if(sound_timer > 0){
        if (sound_timer == 1)
        --sound_timer;
    }
}

bool chip8::loadGame(const char* file) {
    initialize(); 
    
    // game starts at mem[0x200]     
    std::ifstream rom;

    rom.open(file, std::ios::in |  std::ios::ate | std::ios::binary);  
    std::streamsize bufferSize = rom.tellg(); 
    rom.seekg(0, std::ios::beg); 

    std::vector<char> buffer(bufferSize); 

    if (rom.read(buffer.data(), bufferSize)) {
        
        for (int i = 0; i <= buffer.size(); i++) {
            memory[0x200 + i] = buffer[i]; 
        }
    } else {
        std::cout << "Couldn't read file. Exiting..." << "\n"; 
        return false; 
    }

    rom.close();
    return true; 
}
