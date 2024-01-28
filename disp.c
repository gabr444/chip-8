#include "disp.h"

uint32_t pixels[H][W] = {0};

uint8_t keyMap[16] = {0};
SDL_Renderer *renderer;

void initWindow()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("fail\n");
    }
    
    SDL_Window *window = SDL_CreateWindow("CHIP-8",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          W*SIZEUP, H*SIZEUP,
                                          SDL_WINDOW_SHOWN);
    // Main loop.
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void keyPress()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_1:
                        keyMap[0] = 1;
                        break;
                    case SDLK_2:
                        keyMap[1] = 1;
                        break;
                    case SDLK_3:
                        keyMap[2] = 1;
                        break;
                    case SDLK_4:
                        keyMap[3] = 1;
                        break;
                    case SDLK_q:
                        keyMap[4] = 1;
                        break;
                    case SDLK_w:
                        keyMap[5] = 1;
                        break;
                    case SDLK_e:
                        keyMap[6] = 1;
                        break;
                    case SDLK_r:
                        keyMap[7] = 1;
                        break;
                    case SDLK_a:
                        keyMap[8] = 1;
                        break;
                    case SDLK_s:
                        keyMap[9] = 1;
                        break;
                    case SDLK_d:
                        keyMap[10] = 1;
                        break;
                    case SDLK_f:
                        keyMap[11] = 1;
                        break;
                    case SDLK_z:
                        keyMap[12] = 1;
                        break;
                    case SDLK_x:
                        keyMap[13] = 1;
                        break;
                    case SDLK_c:
                        keyMap[14] = 1;
                        break;
                    case SDLK_v:
                        keyMap[15] = 1;
                        break;
                }
                break;
            case SDL_KEYUP:
                switch(event.key.keysym.sym)
                {
                    case SDLK_1:
                        keyMap[0] = 0;
                        break;
                    case SDLK_2:
                        keyMap[1] = 0;
                        break;
                    case SDLK_3:
                        keyMap[2] = 0;
                        break;
                    case SDLK_4:
                        keyMap[3] = 0;
                        break;
                    case SDLK_q:
                        keyMap[4] = 0;
                        break;
                    case SDLK_w:
                        keyMap[5] = 0;
                        break;
                    case SDLK_e:
                        keyMap[6] = 0;
                        break;
                    case SDLK_r:
                        keyMap[7] = 0;
                        break;
                    case SDLK_a:
                        keyMap[8] = 0;
                        break;
                    case SDLK_s:
                        keyMap[9] = 0;
                        break;
                    case SDLK_d:
                        keyMap[10] = 0;
                        break;
                    case SDLK_f:
                        keyMap[11] = 0;
                        break;
                    case SDLK_z:
                        keyMap[12] = 0;
                        break;
                    case SDLK_x:
                        keyMap[13] = 0;
                        break;
                    case SDLK_c:
                        keyMap[14] = 0;
                        break;
                    case SDLK_v:
                        keyMap[15] = 0;
                        break;
                }
                break;
            default:
                break;
        }
    }
}

unsigned char getKey()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        for(int i=0;i<16;i++)
        {
            if(event.key.keysym.sym == keyMap[i])
            {
                return i;
            }
        }
    }

    return -1;
}

void updateScreen()
{
    SDL_Rect rect;
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear(renderer);
    for(int y=0;y<H;y++)
    {
        for(int x = 0;x<W;x++)
        {
            if(pixels[y][x] == 1)
            {
                rect.x = x*SIZEUP;
                rect.y = y*SIZEUP;
                rect.h = SIZEUP;
                rect.w = SIZEUP;
                SDL_SetRenderDrawColor( renderer, 0, 160, 255, 255 );
                SDL_RenderFillRect(renderer, &rect);
                SDL_RenderPresent(renderer);
            }
        }
    }
}

void clearScreen()
{
    for(int i=0;i<H;i++)
    {
        for(int j=0;j<W;j++)
        {
            pixels[i][j] = 0;
        }
    }
}