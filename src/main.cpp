#include "chip8.h"
#include <iostream>
#include "SDL2/SDL.h"

chip8 mychip8; 

const int WINDOW_WIDTH  = 640; 
const int WINDOW_HEIGHT = 320;

SDL_Window *window = NULL; 
SDL_Renderer *renderer = NULL; 
SDL_Texture *texture = NULL;  

bool initWindow() 
{
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
        printf("Could not initialize SDL Error: %s.\n",SDL_GetError()); 
        return false; 
    }; 
    
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
	}

    window = SDL_CreateWindow("emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN); 
    if (window == NULL) {
        printf("Error creating window. SDL Error: %s\n", SDL_GetError()); 
        return false; 
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); 
    if (renderer == NULL) {
        printf("Error creating renderer. SDL Error: %s\n", SDL_GetError()); 
        return false; 
    } 

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);

    return true;
}

void close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    renderer = NULL;
    window = NULL;
    
    SDL_Quit();
}

int main(int argc, char**argv)
{
    if (argc < 2) {
        std::cout << "Usage ./play name_of_ROM" << std::endl; 

    }

    if (!initWindow())
    {
        std::cout << "Failed to initialize window" << std::endl; 
    } else 
        bool quit = false; 
        mychip8.loadGame(argv[1]); 
        
        SDL_Event e; 

        while(!quit) {
            while( SDL_PollEvent( &e ) != 0 ) {
                if( e.type == SDL_QUIT ) {
                    quit = true;
                }
                else if (e.type == SDL_KEYDOWN &&e.key.repeat == 0 ){
                    switch(e.key.keysym.sym){
                        case SDLK_1:
                            mychip8.key[0x1] = 1;
                        break;
                
                        case SDLK_2:
                            mychip8.key[0x2] = 1;
                        break;
            
                        case SDLK_3:
                            mychip8.key[0x3] = 1;
                        break;
            
                        case SDLK_4:
                            mychip8.key[0xC] = 1;
                        break;
            
                        case SDLK_q:
                            mychip8.key[0x4] = 1;
                        break;
            
                        case SDLK_w:
                            mychip8.key[0x5] = 1;
                        break;
            
                        case SDLK_e:
                            mychip8.key[0x6] = 1;
                        break;
            
                        case SDLK_r:
                            mychip8.key[0xD] = 1;
                        break;
            
                        case SDLK_a:
                            mychip8.key[0x7] = 1;
                        break;
            
                        case SDLK_s:
                            mychip8.key[0x8] = 1;
                        break;
            
                        case SDLK_d:
                            mychip8.key[0x9] = 1;
                        break;
            
                        case SDLK_f:
                            mychip8.key[0xE] = 1;
                        break;
            
                        case SDLK_z:
                            mychip8.key[0xA] = 1;
                        break;
                
                        case SDLK_x:
                            mychip8.key[0x0] = 1;
                        break;
            
                        case SDLK_c:
                            mychip8.key[0xB] = 1;
                        break;
            
                        case SDLK_v:
                            mychip8.key[0xF] = 1;
                        break;
                    
                        case SDLK_ESCAPE:
                            exit(0);
                        break;
                    }
                }
                else if(e.type == SDL_KEYUP && e.key.repeat == 0) {
                    switch(e.key.keysym.sym){
                        case SDLK_1:
                            mychip8.key[0x1] = 0;
                        break;
            
                        case SDLK_2:
                            mychip8.key[0x2] = 0;
                        break;
            
                        case SDLK_3:
                            mychip8.key[0x3] = 0;
                        break;
            
                        case SDLK_4:
                            mychip8.key[0xC] = 0;
                        break;
            
                        case SDLK_q:
                            mychip8.key[0x4] = 0;
                        break;
            
                        case SDLK_w:
                            mychip8.key[0x5] = 0;
                        break;
            
                        case SDLK_e:
                            mychip8.key[0x6] = 0;
                        break;
            
                        case SDLK_r:
                            mychip8.key[0xD] = 0;
                        break;
            
                        case SDLK_a:
                            mychip8.key[0x7] = 0;
                        break;
            
                        case SDLK_s:
                            mychip8.key[0x8] = 0;
                        break;
            
                        case SDLK_d:
                            mychip8.key[0x9] = 0;
                        break;
            
                        case SDLK_f:
                            mychip8.key[0xE] = 0;
                        break;
            
                        case SDLK_z:
                            mychip8.key[0xA] = 0;
                        break;
            
                        case SDLK_x:
                            mychip8.key[0x0] = 0;
                        break;
            
                        case SDLK_c:
                            mychip8.key[0xB] = 0;
                        break;
            
                        case SDLK_v:
                            mychip8.key[0xF] = 0;
                        break;     
                    }
                } 
            }
    }
    close(); 

    return 1; 

}