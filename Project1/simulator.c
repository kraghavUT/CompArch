#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MEM_SIZE 512*1024

uint64_t pc = 0;

typedef union Register{
        double dec;
        uint64_t unsignedInt;
        int64_t signedInt;
}Register;

int myExit(int x);
void opcodeFunction(uint8_t * MEMORY, Register * allRegs);
int checkPCRange(uint64_t pc, uint64_t maxSize);

int main(int argc, char** argv){
    Register *allRegs = calloc(32, sizeof(Register));

    uint8_t * MEMORY = calloc(MEM_SIZE, sizeof(uint8_t));

    // for(int i = 0; i < MEM_SIZE; i++)
    // {
    //     MEMORY[i] = (uint8_t) 0;
    // }

    FILE *file;
    file = fopen(argv[1], "rb");
    if (file == NULL) {
          fprintf(stderr, "Invalid tinker filepath");
          return 1;
    }
    uint32_t addMe;
    size_t addMeTemp;
    int index = 0;
    
    // while(fread(&addMe, sizeof(addMe), 4, file)){
    //     MEMORY[index] = addMe;
    //     index++;
    // }
    
    fread(MEMORY, sizeof(uint8_t), MEM_SIZE, file);
    for(int i = 0; i < 31; i++){
        allRegs[i].unsignedInt = 0;
    }
    allRegs[31].unsignedInt = MEM_SIZE;

    while(1){
        //printf("pc: %d", pc);
        opcodeFunction(MEMORY, allRegs);
    }

    return 0;
}
int myExit(int x){
    if(x == 0){
        exit(0);
    }
    fprintf(stderr, "Simulation error\n");
    exit(x);
}

int checkPCRange(uint64_t pc, uint64_t maxSize)
{
    if(pc < MEM_SIZE)
        return 0;
    else 
        return 1;
}

void opcodeFunction(uint8_t * MEMORY, Register * allRegs)
{

    printf("hello %lu", pc);
    //  int ret = checkPCRange(pc, MEM_SIZE);
    //  if(ret = 1)
    //      myExit(ret);

    uint32_t result = 0;
    for (int i = 0; i < 4; i++) {
       // printf("Memory index num %d \n", MEMORY[programCounter + i] );
        result |= MEMORY[pc + i] << (i * 8);
    }
    
    int op = (result>> 27) & 0x1f;
    int rd = (result>> 22) & 0x1f;
    int rs = (result>> 17) & 0x1f;
    int rt = (result>> 12) & 0x1f;
    uint64_t im = result& 0xfff; //ff f 

    printf("opcode: %d \n", op);
    printf("rd: %d \n", rd);
    printf("rs: %d \n", rs);
    printf("rt: %d \n", rt);
    printf("im: %ld \n", im);
    uint64_t mask = 0b1111111111111111111111111111111111111111111111111111000000000000;
    switch(op){
        
        case 0:
            allRegs[rd].signedInt = allRegs[rs].signedInt + allRegs[rt].signedInt;
            pc += 4;
            break;
        case 1:
            allRegs[rd].unsignedInt = allRegs[rd].unsignedInt + allRegs[im].unsignedInt;
             pc += 4;
            break;
        case 2:
            allRegs[rd].signedInt = allRegs[rs].signedInt - allRegs[rt].signedInt;
             pc += 4;
            break;
        case 3:
            allRegs[rd].unsignedInt = allRegs[rd].unsignedInt - allRegs[im].unsignedInt;
             pc += 4;
            break;
        case 4:
            allRegs[rd].signedInt = allRegs[rs].signedInt * allRegs[rt].signedInt;
             pc += 4;
            break;
        case 5:
             pc += 4;
            if(allRegs[rt].signedInt == 0)
            {
                fprintf(stderr, "Simulation error");
            }
            else
                allRegs[rd].signedInt = allRegs[rs].signedInt / allRegs[rt].signedInt;
                
            break;
        case 6:
            allRegs[rd].unsignedInt = allRegs[rs].unsignedInt & allRegs[rt].unsignedInt;
             pc += 4;
            break;
        case 7:
            allRegs[rd].unsignedInt = allRegs[rs].unsignedInt | allRegs[rt].unsignedInt;
             pc += 4;
            break;
        case 8:
            allRegs[rd].unsignedInt = allRegs[rs].unsignedInt ^ allRegs[rt].unsignedInt;
             pc += 4;
            break;
        case 9:
            allRegs[rd].unsignedInt = ~allRegs[rs].unsignedInt;
             pc += 4;
            break;
        case 10:
            allRegs[rd].unsignedInt = allRegs[rs].unsignedInt >> allRegs[rt].unsignedInt;
             pc += 4;
            break;
        case 11:
            allRegs[rd].unsignedInt = allRegs[rd].unsignedInt >> allRegs[im].unsignedInt;
             pc += 4;
            break;
        case 12:
            allRegs[rd].unsignedInt = allRegs[rs].unsignedInt << allRegs[rt].unsignedInt;
             pc += 4;
            break;
        case 13:
            allRegs[rd].unsignedInt = allRegs[rd].unsignedInt << allRegs[im].unsignedInt;
             pc += 4;
            break;
        case 14:
            pc = allRegs[rd].unsignedInt;
            if(checkPCRange(pc, MEM_SIZE)== 1)
            {
                myExit(1);
            }
            break;
        case 15:
            pc = pc + allRegs[rd].unsignedInt;
            if(checkPCRange(pc, MEM_SIZE)== 1){
                myExit(1);
            }
            break;
        case 16:
            pc = pc + allRegs[im].unsignedInt;
            if(checkPCRange(pc, MEM_SIZE)== 1)
            {
                myExit(1);
            }
            break;
        case 17:
            if(allRegs[rs].unsignedInt == 0)
            {
                //check range of pc
                pc = pc + 4;
                if(checkPCRange(pc, MEM_SIZE)== 1)
                {
                    myExit(1);
                }
            }
            else
            {
                pc = allRegs[rd].unsignedInt;
                if(checkPCRange(pc, MEM_SIZE)== 1)
                {
                    myExit(1);
                }
            }
                
            break;
        case 18: 
            MEMORY[allRegs[31].unsignedInt - 8] = pc + 4;
            pc = allRegs[rd].unsignedInt;
            if(checkPCRange(pc, MEM_SIZE)== 1)
            {
                myExit(1);
            }
            break;
        case 19:
            pc = MEMORY[allRegs[31].unsignedInt - 8];
            if(checkPCRange(pc, MEM_SIZE)== 1)
            {
                myExit(1);
            }
            break;
        case 20:
            if(allRegs[rs].unsignedInt <= allRegs[rt].unsignedInt)
            {
                pc = pc + 4;
                if(checkPCRange(pc, MEM_SIZE)== 1)
                {
                    myExit(1);
                }
            }
            else 
            {
                pc = allRegs[rd].unsignedInt;
                if(checkPCRange(pc, MEM_SIZE)== 1)
                {
                    myExit(1);
                }
            }

            break;
        
        case 21:
            allRegs[rd].unsignedInt = MEMORY[allRegs[rs].unsignedInt + allRegs[im].unsignedInt];
             pc += 4;
            break;
        case 22:
            allRegs[rd].unsignedInt = allRegs[rs].unsignedInt;
             pc += 4;
            break;
        case 23: //verify validity on this one
            
            allRegs[rd].unsignedInt &= mask;
            allRegs[rd].unsignedInt |= allRegs[im].unsignedInt; 
             pc += 4;
            break;
        case 24:
            MEMORY[allRegs[rd].unsignedInt + allRegs[im].unsignedInt] = allRegs[rs].unsignedInt;
             pc += 4;
            break;
        case 25:
            allRegs[rd].dec = allRegs[rs].dec + allRegs[rt].dec;
             pc += 4;
            break;
        case 26: 
            allRegs[rd].dec = allRegs[rs].dec - allRegs[rt].dec;
             pc += 4;
            break;
        case 27:
            allRegs[rd].dec = allRegs[rs].dec * allRegs[rt].dec;
             pc += 4;
            break;
        case 28:
            if(allRegs[rt].dec == 0)
            {
                myExit(1);
            }
            allRegs[rd].dec = allRegs[rs].dec / allRegs[rt].dec;
             pc += 4;
            break;
        case 29:
            if(allRegs[rs].unsignedInt == 0)
            {
                uint64_t d;
                scanf("%lu", &d);
                allRegs[rd].unsignedInt = d;
            }
             pc += 4;
            break;
        case 30:
            if(allRegs[rd].unsignedInt == 1)
            {
                uint64_t d;
                printf("%lu \n", allRegs[rs].unsignedInt);
            }
             pc += 4;
        case 31:
            myExit(0);
    }
}







