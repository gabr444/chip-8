#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <time.h>

#define MEMSIZE 4096
#define PC_START 0x200

extern uint8_t memory[MEMSIZE];
extern uint8_t ST;
extern uint8_t DT;
extern uint8_t V[16];
extern uint16_t stack[16];
extern time_t t;

// Program starting index in memory is 0x200 (512).
extern uint16_t I;
extern uint16_t PC;
extern uint8_t SP;
extern uint16_t opcode;

extern uint8_t vy;
extern uint8_t vx;
extern uint8_t nn;
extern uint16_t nnn;

extern uint8_t fontset[80];

void get_op_code();
