#include "cpu.h"
#include "disp.h"

uint8_t memory[MEMSIZE] = {0};
uint8_t ST = 0;
uint8_t DT = 0;
uint8_t V[16];
uint16_t stack[16] = {0};
time_t t;

// Program starting index in memory is 0x200 (512).
uint16_t I= 0;
uint16_t PC = PC_START;
uint8_t SP = 0;
uint16_t opcode;

uint8_t vy;
uint8_t vx;
uint8_t nn;
uint16_t nnn;

uint8_t fontset[80] = 
{
	0xF0, 0x90, 0x90, 0x90, 0xF0,		// 0
	0x20, 0x60, 0x20, 0x20, 0x70,		// 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0,		// 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0,		// 3
	0x90, 0x90, 0xF0, 0x10, 0x10,		// 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0,		// 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0,		// 6
	0xF0, 0x10, 0x20, 0x40, 0x40,		// 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0,		// 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0,		// 9
	0xF0, 0x90, 0xF0, 0x90, 0x90,		// A
	0xE0, 0x90, 0xE0, 0x90, 0xE0,		// B
	0xF0, 0x80, 0x80, 0x80, 0xF0,		// C
	0xE0, 0x90, 0x90, 0x90, 0xE0,		// D
	0xF0, 0x80, 0xF0, 0x80, 0xF0,		// E
	0xF0, 0x80, 0xF0, 0x80, 0x80		// F
};

void get_op_code()
{  
    switch(opcode & 0xF000)
    {
        case 0x0000:
            switch(opcode % 0x00FF)
            {
                case 0x00E0: 
                    // clear screen
                    clearScreen();
                    break;
                case 0x00EE:
                    SP-=1; 
                    PC = stack[SP]; 
                    break;
                default:
                    break;
            }
            break;

        case 0x1000:
            // OPCODE: 1NNN, jump to address NNN. 
            nnn = opcode & 0x0FFF;
            PC = nnn;
           
            break;

        case 0x2000:
            // OPCODE: 2NNN, call subroutine NNN.
            nnn = opcode & 0x0FFF;
            stack[SP] = PC;
            SP+=1;
            PC = nnn;
            break;

        case 0x3000:
            // NN == VX
            nn = (opcode & 0x00FF);
            vx = (opcode>>8) & 0xf;

            if(V[vx] == nn) 
            {
                PC+=2;
            }
            break;

        case 0x4000:
            // NN != VX
            nn = (opcode & 0x00FF);
            vx = (opcode>>8) & 0xf;
            if(V[vx] != nn)
            {
                PC+=2;
            }
            break;

        case 0x5000:
            // Vx == Vy
            vy = (opcode>>4) & 0xf;
            vx = (opcode>>8) & 0xf;
            if(V[vx] == V[vy])
            {
                PC+=2;
            }
            break;

        case 0x6000:
            //6XNN, set Vx == NN
            nn = (opcode & 0x00FF);
            vx = (opcode>>8) & 0xf;
            V[vx] = nn; 
            break;

        case 0x7000:
            //6XNN, set Vx += NN
            nn = (opcode & 0x00FF);
            vx = (opcode>>8) & 0xf;
            V[vx] += nn; 
            break;

        case 0x8000:
            switch(opcode & 0xf00f)
            {
                case 0x8000:
                    vy = (opcode>>4) & 0xf;
                    vx = (opcode>>8) & 0xf;
                    V[vx] = V[vy];
                    break;

                case 0x8001:
                    vy = (opcode>>4) & 0xf;
                    vx = (opcode>>8) & 0xf;
                    V[vx]|= V[vy];
                    break;

                case 0x8002:
                    vy = (opcode>>4) & 0xf;
                    vx = (opcode>>8) & 0xf;
                    V[vx] &= V[vy];
                    break;

                case 0x8003:
                    V[(opcode>>8) & 0xf] ^= V[(opcode>>4) & 0xf];
                    break;
                
                case 0x8004:
                    vy = (opcode>>4) & 0xf;
                    vx = (opcode>>8) & 0xf;
                    if(vx+vy > 255)
                    {
                        V[0xf] = 1;
                    }
                    else
                    {
                        V[0xf] = 0;
                    }
                    V[vx] +=vy;
                    break;

                case 0x8005:
                    vy = (opcode>>4) & 0xf;
                    vx = (opcode>>8) & 0xf;
                    if(V[vx] > V[vy])
                    {
                        V[0xf] = 1;
                    }
                    else
                    {
                        V[0xf] = 0;
                    }

                    V[vx] -= V[vy];
                    break;
                
                case 0x8006:
                    vx = (opcode>>8) & 0xf;
                    V[0xF] = (V[vx] & 0x1);
                    V[vx] >>= 1;
                    break;

                case 0x8007:
                    vy = (opcode>>4) & 0xf;
                    vx = (opcode>>8) & 0xf;
                    if(V[vy] > V[vx])
                    {
                        V[0xf] = 1;
                    }
                    else
                    {
                        V[0xf] = 0;
                    }
                    V[vx] = V[vy]-V[vx];
                    break;
                
                case 0x800E:
                    vx = (opcode>>8) & 0xf;
                    V[0xF] = (V[vx] & 0x80) >> 7;
                    V[vx] <<=1;
                    break;
                default:
                    break;
            }
            break;
           

        case 0x9000:
            vy = (opcode>>4) & 0xf;
            vx = (opcode>>8) & 0xf;
            if(V[vx] != V[vy])
            {
                PC+=2;
            }
            break;

        case 0xA000:
            nnn = opcode & 0x0FFF;
            I = nnn;
            break;

        case 0xB000:
            // PC = V0 + NNN
            nnn = opcode & 0x0FFF;
            PC = V[0] + nnn;
            break;

        case 0xC000:
            vy = (opcode>>4) & 0xf;
            nn = opcode & 0x00FF;
            srand(time(NULL));
            const int randByte = rand() % 256;
            V[vx] = nn & (randByte);
            break;

        case 0xD000:
            // Draw sprite
            vy = (opcode>>4) & 0xf;
            vx = (opcode>>8) & 0xf;
            uint8_t x = V[vx] % 64;
            uint8_t y = V[vy] % 32;
            uint8_t n = opcode & 0xf;

            V[0xF] = 0;
            for (uint8_t row = 0; row < n; ++row)
	        {
		        uint8_t byte = memory[I + row];

		        for (uint8_t col = 0; col < 8; ++col)
		        {
                    uint8_t spritePixel = byte & (0x80 >> col);
                    if(spritePixel)
                    {
                        if(pixels[y+row][x+col] == 1)
                        {
                            V[0xF] = 1; 
                        }
                        
                        pixels[y+row][x+col] ^= 1;
                    }
		        }
	        }

            break;

        case 0xE000:
            switch(opcode & 0xF0FF)
            {
                case 0xE09E:
                    vx = (opcode>>8) & 0xf;
                    if(keyMap[vx] == 1)
                    {
                        PC+=2;
                    }
                    break;

                case 0xE0A1:
                    vx = (opcode>>8) & 0xf;
                    if(keyMap[vx] == 0)
                    {
                        PC+=2;
                    }
                    break;
                default:
                    break;
            }
            break;

        case 0xF000:
            switch(opcode & 0xF0FF)
            {
                case 0xF007:
                    
                    vx = (opcode>>8) & 0xf;
                    V[vx] = DT;
                    break;

                case 0xF00A:
                    vx = (opcode>>8) & 0xf;
                    V[vx] = getKey();
                    break;
                
                case 0xF015:
                    vx = (opcode>>8) & 0xf;
                    DT = V[vx];
                    break;

                case 0xF018:
                    vx = (opcode>>8) & 0xf;
                    ST = V[vx];
                    break;
                case 0xF01E:
                    vx = (opcode>>8) & 0xf;
                    if(I>0xfff)
                    {
                        V[0xf] = 1;
                    }
                    I+=V[vx];
                    break;

                case 0xF029:
                    vx = (opcode>>8) & 0xf;
                    I = 0x50+(5*V[vx]);
                    break;
                
                case 0xF033:
                    vx = (opcode & 0x0F00) >> 8;
	                uint8_t value = V[vx];

	                // Ones-place
	                memory[I + 2] = value % 10;
	                value /= 10;

	                // Tens-place
	                memory[I + 1] = value % 10;
	                value /= 10;

                    // Hundreds-place
                    memory[I] = value % 10;
                    break;
                
                case 0xF055: 
                    for(uint8_t i=0;i<=(opcode>>8) & 0xf+1;i++)
                    {
                        memory[I+i] = V[i];
                    }
                    break;
                
                case 0xF065:
                    for(uint8_t i=0;i<=((opcode>>8) & 0xf);i++)
                    {
                        V[i] = memory[I+i];
                    }
                    break;
                default:
                    break;
                
            }
            break;

        default: 
            break;
    }
}