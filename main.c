#include "cpu.h"
#include "disp.h"
#include <dirent.h>

void readRomNames(char folder[])
{
    DIR *d;
    struct dirent *dir;
    d = opendir(folder);
    if (d)
    {
        int idx = 0;
        while ((dir = readdir(d)) != NULL)
        {
            if(idx > 1)
            {
                printf("%s\n", dir->d_name);
            }
            idx+=1;
        }
        closedir(d);
    }
}

int main(int argc, char *argv[])
{
    char folder[] = "rom/";
    printf("Files to load:\n");
    readRomNames(folder);
    printf("\nEnter file name: ");
    char filename[256];
    fgets(filename, 256, stdin);

    strcat(folder, filename);
    char *newline = strchr( folder, '\n' );
    if (newline)
    {
        *newline = 0;
    }

    FILE *fp = fopen(folder, "rb");

    if(fp == NULL)
    {
        printf("Error: File not found!");
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    long rom_size = ftell(fp);
    rewind(fp);

    // Allocate memory to store rom
    char* rom_buffer = (char*) malloc(sizeof(char) * rom_size);

    // Copy ROM into buffer
    size_t result = fread(rom_buffer, sizeof(char), (size_t)rom_size, fp);
    
    // Close file pointer
    fclose(fp);

    for(int i = 0; i<rom_size; i++)
    {
        // Program storage goes from address 0x200 and upwards. 
        memory[PC+i] = rom_buffer[i];
    }


    // Load font
    for(int i=0;i<80;i++)
    {
        memory[i+0x50] = fontset[i];
    }

    initWindow();
    
    int cycle = 0;
    printf("\nPress P to quit!\n");
    while (!quit)
    {
        keyPress();
        if(cycle % 9 == 0)
        {
            updateScreen();
            SDL_Delay(50);
        }
        opcode = memory[PC] << 8 | memory[PC+1];
        //printf("%x\n", opcode);
        PC+=2;
        get_op_code();
        if(DT > 0)
        {
            DT-=1;
        };
        if(ST > 0)
        {
            ST-=1;
        }
        cycle+=1;
    }
    
    SDL_Quit();

    return 0;
}