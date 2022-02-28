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
        void initialize(); 

        bool loadGame(const char* file); 

        void runCycle(); 

        // Array to store state of pixels
        uint8_t graphics[64 * 32]; 

        // Array to store state of key inputs
        uint8_t key[16];

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