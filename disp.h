#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define H 32
#define W 64
#define SIZEUP 25

extern uint32_t pixels[H][W];

extern uint8_t keyMap[16];

extern bool quit;

extern SDL_Renderer *renderer;

void keyPress();

unsigned char getKey();

void initWindow();

void updateScreen();

void clearScreen();