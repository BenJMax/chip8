#ifndef CHIP8
#define CHIP8
#include <stdint.h>

class chip8
{

    public:
        /*
        Set the initial values of PC, I, etc.
        Load sprites to memory at address 0x050-0x0A0
        */ 
        void chip8::initialize(); 

        void chip8::loadGame(); 

        void chip8::runCycle(); 

        /*
        Thanks to Lawrence Mueller @ multigesture.net for 
        providing the chip8 font sprites
        */
        uint8_t chip8_fontset[80] =  
        { 
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

    private:
        // Program counter
        uint16_t PC;

        // Registers
        //  Index register
        uint16_t I; 
        //  Genral purpose registers V0 - VF (V[15])
        uint8_t V[16];

        // Opcode 
        uint16_t opcode; 

        // Memory
        uint8_t memory[4096];

        // Timers for sound and game delay
        uint8_t sound_timer; 
        uint8_t delay_timer; 

        // Stack for subroutines and stack pointer
        uint16_t stack[16];
        uint16_t sp; 

}; 
#endif