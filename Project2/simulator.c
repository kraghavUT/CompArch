#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <float.h>

#define MEM_SIZE 512*1024

uint32_t pc = 0x0;
int sp = 0x40000;

void opcodeFunction(uint8_t * MEMORY);
int myExit(int x);
void verifyAddress(uint32_t address);

char getCharInput();
int getByteInput();
int getShortInput();
int getIntInput();
long getLongInput();
float getFloatInput();
double getDoubleInput();
int8_t adjustForSign(int8_t num);

void setInt8Nums(int8_t *mem1, int8_t* mem2, int8_t * MEMORY);
// void get16BitValue(uint8_t * MEMORY, uint16_t* val);
void setInt16Nums(int16_t *mem1_16, int16_t* mem2_16, int8_t * MEMORY);
void setInt32Nums(int32_t *mem1_32, int32_t *mem2_32, int8_t * MEMORY);
void setInt64Nums(int64_t *mem1_64, int64_t *mem2_64, int8_t * MEMORY);

int16_t getInt16Num(uint8_t *MEMORY, int starting);

int16_t to16(int8_t num);
int32_t to32(int8_t num);
int64_t to64(int8_t num);
float toFloat(int8_t num);
double toDouble(int8_t num);

int8_t to8From16(int16_t num);
int32_t to32From16(int16_t num);
int64_t to64From16(int16_t num);
float toFloatFrom16(int16_t num);
double toDoubleFrom16(int16_t num);

int8_t to8From32(int32_t num);
int16_t to16From32(int32_t num);
int64_t to64From32(int32_t num);
float toFloatFrom32(int32_t num);
double toDoubleFrom32(int32_t num);

int8_t to8From64(int64_t num);
int16_t to16From64(int64_t num);
int32_t to32From64(int64_t num);
float toFloatFrom64(int64_t num);
double toDoubleFrom64(int64_t num);

int8_t to8FromFloat(float num);
int16_t to16FromFloat(float num);
int32_t to32FromFloat(float f1);
int64_t to64FromFloat(float num);
double toDoubleFromFloat(float num);

int8_t to8FromDouble(int64_t num);
int16_t to16FromDouble(int64_t num);
int32_t to32FromDouble(int64_t num);
int64_t to64FromDouble(int64_t num);
float toFloatFromDouble(int64_t num);



int8_t readInt8(uint8_t * MEMORY, int loc);
int16_t readInt16(uint8_t *MEMORY, int loc);
int32_t readInt32(uint8_t *MEMORY, int loc);
int64_t readInt64(uint8_t *MEMORY, int loc);
float readFloat(uint8_t *MEMORY, int loc);
double readDouble(uint8_t *MEMORY, int loc);

int8_t readInt8Address(uint8_t *MEMORY, uint32_t addr);
int16_t readInt16Address(uint8_t *MEMORY, uint32_t addr);
int32_t readInt32Address(uint8_t *MEMORY, uint32_t addr);
int64_t readInt64Address(uint8_t *MEMORY, uint32_t addr);
float readFloatAddress(uint8_t *MEMORY, uint32_t addr);
double readDoubleAddress(uint8_t *MEMORY, uint32_t addr);


void writeInt8Num(int8_t num, uint8_t * MEMORY, int loc);
void writeInt16Num(int16_t num, uint8_t * MEMORY, int loc);
void writeInt32Nums(int32_t num, uint8_t * MEMORY, int loc);
void writeInt64Nums(int64_t num, uint8_t * MEMORY, int loc);
void writeFloatNum(float tempF, uint8_t * MEMORY, int loc);
void writeDoubleNum(double tempD, uint8_t * MEMORY, int loc);

void writeInt8Address(int8_t num, uint8_t * MEMORY, uint32_t addr);
void writeInt16Address(int16_t num, uint8_t * MEMORY, uint32_t addr);
void writeInt32Address(int32_t num, uint8_t * MEMORY, uint32_t addr);
void writeInt64Address(int64_t num, uint8_t * MEMORY, uint32_t addr);
void writeFloatAddress(float num, uint8_t * MEMORY, uint32_t addr);
void writeDoubleAddress(double num, uint8_t * MEMORY, uint32_t addr);


uint32_t readAddress4Bytes(uint8_t * MEMORY);
uint32_t readAddress3Bytes(uint8_t * MEMORY);
void writeAddress(uint32_t address, uint8_t * MEMORY);
uint32_t readAddFromStack(uint8_t * MEMORY);

uint8_t readUInt8(uint8_t *MEMORY, int loc);

uint32_t readUInt32(uint8_t *MEMORY, int loc);
uint64_t readUInt64(uint8_t *MEMORY, int loc);

uint8_t readUInt8PC(uint8_t * MEMORY);
uint16_t readUInt16PC(uint8_t *MEMORY);
uint32_t readUInt32PC(uint8_t * MEMORY);
uint64_t readUInt64PC(uint8_t * MEMORY);
float readFloatPC(uint8_t * MEMORY);
double readDoublePC(uint8_t * MEMORY);




void writeUInt32Nums(uint32_t num, uint8_t * MEMORY, int loc);

int main(int argc, char** argv){

    uint8_t * MEMORY = calloc(MEM_SIZE, sizeof(uint8_t));
    uint8_t * HEADER = calloc(8, sizeof(uint8_t));

    FILE *file;
    file = fopen(argv[1], "rb");
    if (file == NULL) {
          fprintf(stderr, "Invalid tinker filepath");
          return 1;
    }

    //collect the header information and save offsets
    fread(HEADER, sizeof(uint8_t), 8, file);
    int codeOffset = HEADER[3];
    int dataOffset = HEADER[7];

    //place the rest of the data excluding header into memory
    if (fseek(file, 8, SEEK_SET) != 0) {
        perror("Error seeking file");
        fclose(file);
        myExit(1);
    }
    fread(MEMORY, sizeof(uint8_t), MEM_SIZE, file);

    
    while(1){
        opcodeFunction(MEMORY);
        // printf("pc now: %d \n", pc);
        // printf("sp now: %d \n", sp);
    }

    return 0;
}
//put stuff into memory[sp]
void opcodeFunction(uint8_t * MEMORY){

    int8_t OPCODE = MEMORY[pc];
    //printf("opcode: %d \n", OPCODE);

    int op;
    if(OPCODE >= 0){
        op = OPCODE;
        //printf("hai:\n");
    }
    else{
        op = (int) OPCODE + 256;
    }

    int8_t eightBitValue;
    int8_t eightBitTempI;
    uint8_t eightBitTemp;
    uint32_t address;

    uint8_t tmp;

    int16_t sixteenTemp;
    int16_t sixteenTemp2;
    int32_t threetwoTemp;
    int32_t threetwoTemp2;
    int64_t sixfourTemp;
    int64_t sixfourTemp2;

    int8_t mem1, mem2;
    uint8_t data;
    int16_t mem1_16, mem2_16;
    int32_t mem1_32, mem2_32;
    int64_t mem1_64, mem2_64;

    uint16_t comb;
    uint16_t res;

    uint8_t shortByte1;
    uint8_t shortByte2;
    uint16_t fullShort1;

    uint8_t shortByte3;
    uint8_t shortByte4;
    uint16_t fullShort2;
    uint16_t fullShort3;
    uint16_t fullShort4;

    uint32_t fullInt1;
    uint32_t fullInt2;
    uint32_t fullInt3;

    uint64_t fullLong1;
    uint64_t fullLong2;
    uint64_t fullLong3;


    float tempF;
    float f1;
    float f2;
    double tempD;
    double d1;
    double d2;
    int findChar;
    int change;
    long changeL;
    
    
    switch(op){
        case 0:
            eightBitTemp = readUInt8PC(MEMORY);
            sp+=1;
            writeInt8Num(eightBitTemp, MEMORY, 1);            
            pc += 2;
            break;
        case 1:
            fullShort1 = readUInt16PC(MEMORY);
            sp+=2;
            writeInt16Num(fullShort1, MEMORY, 2);
            pc += 3; 
            break;
        case 2:
            fullInt1 = readUInt32PC(MEMORY);
            sp = sp + 4; 
            writeInt32Nums(fullInt1, MEMORY, 4);
            pc = pc + 5;
            break;
        case 3: 
            MEMORY[sp] = MEMORY[pc+1]; 
            MEMORY[sp+1] = MEMORY[pc+2];
            MEMORY[sp+2] = MEMORY[pc+3];
            MEMORY[sp+3] = MEMORY[pc+4];
            MEMORY[sp+4] = MEMORY[pc+5];
            MEMORY[sp+5] = MEMORY[pc+6];
            MEMORY[sp+6] = MEMORY[pc+7];
            MEMORY[sp+7] = MEMORY[pc+8];
            sp = sp + 8;
            pc = pc + 9;
            break;
            break;
        case 4:
            tempF = readFloatPC(MEMORY);
            sp = sp + 4; 
            writeFloatNum(tempF, MEMORY, 4);
            pc = pc + 5;
            break;
        case 5:
            tempD = readDoublePC(MEMORY);
            sp = sp + 8; 
            writeDoubleNum(tempD, MEMORY, 8);
            pc = pc + 9;
            break;
        case 6:
            address = readAddress3Bytes(MEMORY);
            verifyAddress(address);
            eightBitValue = readInt8Address(MEMORY, address);
            sp+=1;
            writeInt8Num(eightBitValue, MEMORY, 1);
            
            pc+=4;
            break;
        case 7:
            address = readAddress3Bytes(MEMORY);
            verifyAddress(address);
            sixteenTemp  = readInt16Address(MEMORY, address);
            sp+=2;
            writeInt16Num(sixteenTemp, MEMORY, 2);
            pc+=4;
            break;
        case 8:
            address = readAddress3Bytes(MEMORY);
            verifyAddress(address);
            threetwoTemp = readInt32Address(MEMORY, address);
           // printf("threetwo %d\n", threetwoTemp);
            sp+=4;
            writeInt32Nums(threetwoTemp, MEMORY, 4);
            pc+=4;
            break;
        case 9:
            address = readAddress3Bytes(MEMORY);
            verifyAddress(address);
            sixfourTemp = readInt64Address(MEMORY, address);
            sp+=8;
            writeInt64Nums(sixfourTemp, MEMORY, 8);
            pc+=4;
            break;
        case 10:
            address = readAddress3Bytes(MEMORY);
            verifyAddress(address);
            tempF = readFloatAddress(MEMORY, address);
            sp+=4;
            writeFloatNum(tempF, MEMORY, 4);
            pc+=4;
            break;
        case 11:
            address = readAddress3Bytes(MEMORY);
            verifyAddress(address);
            tempD = readDoubleAddress(MEMORY, address);
            sp+=8;
            writeDoubleNum(tempD, MEMORY, 8);
            pc+=4;
            break;
        case 12:
            address = readAddress3Bytes(MEMORY);
            verifyAddress(address);
            eightBitTemp = MEMORY[pc +4];
            for(int i = 0; i < eightBitTemp; i++) { 
                MEMORY[sp + i] = MEMORY[address + i];
            }
            sp = sp + eightBitTemp; 
            pc = pc + 5;
            break;
        case 13:
            eightBitValue = readInt8(MEMORY, 1);
            sp = sp + 1; 
            writeInt8Num(eightBitValue, MEMORY, 1);
            pc = pc + 1;
            break;
        case 14:
            sixteenTemp = readInt16(MEMORY, 2);
            sp = sp + 2;
            writeInt16Num(sixteenTemp, MEMORY, 2);
            pc = pc + 1;
            break;
        case 15:
            threetwoTemp = readInt32(MEMORY, 4);
            sp = sp + 4; 
            writeInt32Nums(threetwoTemp, MEMORY, 4);
            pc = pc + 1;
            break;
        case 16:
            sixfourTemp = readInt64(MEMORY, 8);
            sp = sp + 8; 
            writeInt64Nums(sixfourTemp, MEMORY, 8);
            pc = pc + 1;
            break;
        case 17:
            tempF = readFloat(MEMORY, 4);
            sp = sp + 4; 
            writeFloatNum(tempF, MEMORY, 4);
            pc = pc + 1;
            break;
        case 18:
            tempD = readDouble(MEMORY, 8);
            sp = sp + 8; 
            writeDoubleNum(tempD, MEMORY, 8);
            pc = pc + 1;
            break;
        case 19:
            sp = sp - 1;
            // printf("MEmo: %d", MEMORY[sp]);
            pc = pc + 1;
            break;
        case 20:
            sp = sp - 2; 
            pc = pc + 1;
            break;
        case 21:
            sp = sp - 4; 
            pc = pc + 1;
            break;
        case 22:
            sp = sp - 8; 
            pc = pc + 1;
            break;
        case 23:
            sp = sp - 4;
            pc = pc + 1;
            break;
        case 24:
            sp = sp - 8; 
            pc = pc + 1;
            break;
        case 25:
            eightBitValue = readInt8(MEMORY, 1);
            address = readAddress3Bytes(MEMORY);
            verifyAddress(address);
            sp = sp - 1;
        //  MEMORY[address] = eightBitValue;
            writeInt8Address(eightBitValue, MEMORY, address);
            pc = pc + 4;
            break;
        case 26:
            sixteenTemp = readInt16(MEMORY, 2);
            address = readAddress3Bytes(MEMORY);
            verifyAddress(address);
            sp = sp - 2; 
            writeInt16Address(sixteenTemp, MEMORY, address);
            pc = pc + 4;
            break;
        case 27:
            threetwoTemp = readInt32(MEMORY, 4);
            address = readAddress3Bytes(MEMORY);
            verifyAddress(address);
            // printf("address val: %d\n", address);
            sp = sp - 4;
            writeInt32Address(threetwoTemp, MEMORY, address); 
            // printf("address: %d \n", MEMORY[address]);
            // printf("address+1: %d \n", MEMORY[address+1]);
            // printf("address+2: %d \n", MEMORY[address+2]);
            // printf("address+3: %d \n", MEMORY[address+3]);

            pc = pc + 4;
            break;
        case 28:
            sixfourTemp = readInt64(MEMORY, 8);
            address = readAddress3Bytes(MEMORY);
            verifyAddress(address);
            sp = sp - 8; 
            writeInt64Address(sixfourTemp, MEMORY, address);
            pc = pc + 4;
            break;
        case 29:
            tempF = readFloat(MEMORY, 4);
            address = readAddress3Bytes(MEMORY);
            verifyAddress(address);
            sp = sp - 4; 
            writeFloatAddress(tempF, MEMORY, address);
            pc = pc + 4;
            break;
        case 30:
            tempD = readDouble(MEMORY, 4);
            address = readAddress3Bytes(MEMORY);
            verifyAddress(address);
            sp = sp - 8; 
            writeDoubleAddress(tempD, MEMORY, address);
            pc = pc + 4;
            break;
        case 31:  
            address = readAddress3Bytes(MEMORY);
            verifyAddress(address);
            eightBitTemp = MEMORY[pc+4];
            sp = sp - eightBitTemp; 
            for(int i = 0; i < eightBitTemp; i++){
                MEMORY[address + i] = MEMORY[sp + i];
            }
            pc = pc + 5;
            break;
            //swap cases
        case 32:

            // printf("sp -2 oG %d \n",MEMORY[sp - 2]);
            // printf("sp -1 og %d \n",MEMORY[sp - 1]);
            tmp = MEMORY[sp - 1]; 
            MEMORY[sp - 1] = MEMORY[sp - 2];
            MEMORY[sp - 2] = tmp;
            // printf("sp -2 %d \n",MEMORY[sp - 2]);
            // printf("sp -1 %d \n",MEMORY[sp - 1]);
            pc = pc + 1;
            break;
        case 33:
            sixteenTemp = readInt16(MEMORY, 2); 
            sixteenTemp2 = readInt16(MEMORY, 4);
            writeInt16Num(sixteenTemp2, MEMORY, 2);
            writeInt16Num(sixteenTemp, MEMORY, 4);
            pc = pc + 1;
            break;
        case 34:
            threetwoTemp = readInt32(MEMORY, 4); 
            threetwoTemp2 = readInt32(MEMORY, 8);
            writeInt32Nums(threetwoTemp2, MEMORY, 4);
            writeInt32Nums(threetwoTemp, MEMORY, 8);
            pc = pc + 1;
            break;
        case 35:
            sixfourTemp = readInt64(MEMORY, 8); 
            sixfourTemp2 = readInt64(MEMORY, 16);
            writeInt64Nums(sixfourTemp2, MEMORY, 8);
            writeInt64Nums(sixfourTemp, MEMORY, 16);
            pc = pc + 1;
            break;
        case 36:
            f1 = readFloat(MEMORY, 4); 
            f2 = readFloat(MEMORY, 8);
            writeFloatNum(f2, MEMORY, 4);
            writeFloatNum(f1, MEMORY, 8);
            pc = pc + 1;
            break;

        case 37:
            d1 = readDouble(MEMORY, 8); 
            d2 = readDouble(MEMORY, 16);
            writeDoubleNum(d2, MEMORY, 8);
            writeDoubleNum(d1, MEMORY, 16);
            pc = pc + 1;
            break;
        //conv cases: 38-55, 57-68
        case 38:
            eightBitValue = readInt8(MEMORY, 1);
            sixteenTemp = to16(eightBitValue);
            sp+=1;
            writeInt16Num(sixteenTemp, MEMORY, 2);
            // printf("MEM-2: %d \n", MEMORY[sp-2]);
            // printf("MEM-1: %d \n", MEMORY[sp-1]);

            pc = pc + 1;
            break;
        case 39:
            //eightBitValue = readInt8(MEMORY, 1);     
            change = to32(MEMORY[sp-1]);
            sp += 3;
            writeInt32Nums(change, MEMORY, 4);
            // printf("sp-4: %d \n", MEMORY[sp - 4]);
            // printf("sp-3: %d \n", MEMORY[sp - 3]);
            // printf("sp-2: %d \n", MEMORY[sp - 2]);
            // printf("sp-1: %d \n", MEMORY[sp - 1]);
            pc+= 1;
            break;

        case 40:
            // eightBitValue = readInt8(MEMORY, 1);
            // sixfourTemp = to64(eightBitValue); 
            // printf("new num: %d \n", sixteenTemp);
            changeL = to64(MEMORY[sp-1]);
            sp+= 7;
            writeInt64Nums(changeL, MEMORY, 8);
            // printf("sp-8: %d \n", MEMORY[sp - 8]);
            // printf("sp-7: %d \n", MEMORY[sp - 7]);
            // printf("sp-6: %d \n", MEMORY[sp - 6]);
            // printf("sp-5: %d \n", MEMORY[sp - 5]);
            // printf("sp-4: %d \n", MEMORY[sp - 4]);
            // printf("sp-3: %d \n", MEMORY[sp - 3]);
            // printf("sp-2: %d \n", MEMORY[sp - 2]);
            // printf("sp-1: %d \n", MEMORY[sp - 1]);
            pc += 1;
            break;

        case 41:
            tempF = toFloat(MEMORY[sp -1]);
            // printf("tempf: %f", tempF);
            sp = sp + 3; 
            writeFloatNum(tempF, MEMORY, 4);
            // printf("sp-4: %d \n", MEMORY[sp - 4]);
            // printf("sp-3: %d \n", MEMORY[sp - 3]);
            // printf("sp-2: %d \n", MEMORY[sp - 2]);
            // printf("sp-1: %d \n", MEMORY[sp - 1]);
            pc = pc + 1;
            break;
        case 42:
            tempD = toDouble(MEMORY[sp-1]);
            sp += 7;
            writeDoubleNum(tempD, MEMORY, 8);
            pc += 1;
            break;
        //convs
        case 43:
            sixteenTemp  = readInt16(MEMORY, 2);
            eightBitValue = to8From16(sixteenTemp);
            sp = sp-1;
            writeInt8Num(eightBitValue, MEMORY, 1);
            pc += 1;
            break;
            
        case 44:
            sixteenTemp = readInt16(MEMORY, 2);
            threetwoTemp= to32From16(sixteenTemp); 
            sp = sp + 2; 
            writeInt32Nums(threetwoTemp, MEMORY, 4);
            pc = pc + 1;
            break;
        case 45:
            sixteenTemp = readInt16(MEMORY, 2);
            sixfourTemp= to64From16(sixteenTemp); 
            sp = sp + 6; 
            writeInt64Nums(sixfourTemp, MEMORY, 8);
            pc = pc + 1;
            break;
        case 46:
            sixteenTemp = readInt16(MEMORY, 2);
            tempF = toFloatFrom16(sixteenTemp);
            sp = sp + 2;
            writeFloatNum(tempF, MEMORY, 4);
            pc+=1;
            break;
        case 47:
            sixteenTemp = readInt16(MEMORY, 2);
            tempD = toDoubleFrom16(sixteenTemp);
            sp = sp + 6;
            writeDoubleNum(tempD, MEMORY, 8);
            pc+=1;
            break;
        //convi
        case 48:
            threetwoTemp = readInt32(MEMORY, 4);
            eightBitValue = to8From32(threetwoTemp);
            writeInt8Num(eightBitValue, MEMORY, 1);
            sp = sp - 3; 
            pc = pc + 1;
            break;

        case 49:
            threetwoTemp = readInt32(MEMORY, 4);
            sixteenTemp = to16From32(threetwoTemp);
            writeInt16Num(sixteenTemp, MEMORY, 2);
            sp = sp - 2; 
            pc = pc + 1;
            break;

        case 50:
            threetwoTemp = readInt32(MEMORY, 4);
            sixfourTemp = to64From32(threetwoTemp);
            sp = sp + 4; 
            writeInt64Nums(sixfourTemp, MEMORY, 8);
            pc = pc + 1;
            break;

        case 51:
            threetwoTemp = readInt32(MEMORY, 4);
            tempF = toFloatFrom32(threetwoTemp);
            writeFloatNum(tempF, MEMORY, 4);
            pc = pc + 1;
            break;

        case 52:
            threetwoTemp = readInt32(MEMORY, 4);
            tempD = toDoubleFrom32(threetwoTemp);
            sp = sp + 4;
            writeDoubleNum(tempD, MEMORY, 8);
            pc = pc + 1;
            break;

        //convl
        case 53:
            sixfourTemp = readInt64(MEMORY, 8);
            eightBitValue = to8From64(sixfourTemp);
            sp = sp - 7; 
            writeInt8Num(eightBitValue, MEMORY, 1);
            pc = pc + 1;
            break;

        case 54:
            sixfourTemp = readInt64(MEMORY, 8);
            sixteenTemp  = to16From64(sixfourTemp);
            sp = sp - 6;
            writeInt16Num(sixteenTemp, MEMORY, 2);
            pc = pc + 1;
            break;
        case 55:
            sixfourTemp = readInt64(MEMORY, 8);
            threetwoTemp = to32From64(sixfourTemp);
            sp = sp - 4;
            writeInt32Nums(threetwoTemp, MEMORY, 4); 
            pc = pc + 1;
            break;

        case 57:
            sixfourTemp = readInt64(MEMORY, 8);
            tempF = toFloatFrom64(sixfourTemp);
            sp = sp - 4;
            writeFloatNum(tempF, MEMORY, 4); 
            pc = pc + 1;
            break;

        case 58:
            sixfourTemp = readInt64(MEMORY, 8);
            tempD = toDoubleFrom64(sixfourTemp);
            writeDoubleNum(tempD, MEMORY, 8); 
            pc = pc + 1;
            break;

        //convf
        case 59:
            tempF = readFloat(MEMORY, 4);
            eightBitValue = to8FromFloat(tempF);
            sp = sp - 3; 
            writeInt8Num(eightBitTemp, MEMORY, 1);
            pc = pc + 1;
            break;

        case 60:
            tempF = readFloat(MEMORY, 4);
            sixteenTemp = to16FromFloat(tempF);
            sp = sp - 2; 
            writeInt16Num(sixteenTemp, MEMORY, 2);
            pc = pc + 1;
            break;

        case 61:
            tempF = readFloat(MEMORY, 4);
            threetwoTemp = to32FromFloat(tempF);
            writeInt32Nums(threetwoTemp, MEMORY, 4);
            pc = pc + 1;
            break;
        case 62:
            tempF = readFloat(MEMORY, 4);
            sixfourTemp = to64FromFloat(tempF);
            sp+=4;
            writeInt64Nums(sixfourTemp, MEMORY, 8);
            pc+=1;
            break;

        case 63:
            tempF = readFloat(MEMORY, 4);
            tempD = toDoubleFromFloat(tempF);
            sp+=4;
            writeDoubleNum(tempD, MEMORY, 8);
            pc+=1;
            break;
        //convd
        case 64:
            tempD = readDouble(MEMORY, 8);
            eightBitValue = to8FromDouble(tempD);
            sp-=7;
            writeInt8Num(eightBitTemp, MEMORY, 1);
            pc = pc + 1;
            break;
        case 65:
            tempD = readDouble(MEMORY, 8);
            sixteenTemp = to16FromDouble(tempD);
            sp-=6;
            writeInt16Num(sixteenTemp, MEMORY, 2);
            pc+=1;
            break;

        case 66:     
            tempD = readDouble(MEMORY, 8);  
            threetwoTemp = to32FromDouble(tempD); 
            sp-=4;
            writeInt32Nums(threetwoTemp, MEMORY, 4);
            pc+=1;
            break;

        case 67:
            tempD = readDouble(MEMORY, 8);
            sixfourTemp = to64FromDouble(tempD);
            writeInt64Nums(sixfourTemp, MEMORY, 8);
            pc+=1;
            break;

        case 68:
            tempD = readDouble(MEMORY, 8);
            tempF = toFloatFromDouble(tempD);
            sp-=4;
            writeFloatNum(tempF, MEMORY, 4);
            pc+=1;
            break;
            
        //in cases
        case 69:
            findChar = getCharInput();
            MEMORY[sp] = findChar;
            sp = sp + 1; 
            pc = pc + 1;
            break;
        case 70:
            eightBitValue = getByteInput();
            MEMORY[sp] = eightBitValue;
            sp = sp + 1; 
            pc = pc + 1;
            break;
        case 71:
            sixteenTemp = getShortInput();
            sp = sp + 2; 
            writeInt16Num(sixteenTemp, MEMORY, 2);
            pc = pc + 1;
            break;
        case 72:
            threetwoTemp = getIntInput();
           // printf("hi: %d \n", threetwoTemp);
            sp = sp + 4;
            writeInt32Nums(threetwoTemp, MEMORY, 4);
            pc = pc + 1;
            break;
        case 73:
            sixfourTemp = getLongInput();
            sp = sp + 8;
            writeInt64Nums(sixfourTemp, MEMORY, 8);
            pc = pc + 1;
            break;
        case 74:
            tempF = getFloatInput();
            sp = sp + 4; 
            writeFloatNum(tempF, MEMORY, 4);
            pc = pc + 1;
            break;
        case 75:
            tempD = getDoubleInput();
            sp = sp + 8; 
            writeDoubleNum(tempD, MEMORY, 8);
            pc = pc + 1;
            break;

        //out cases
        case 76:
            sp = sp - 1;
            printf("%c \n", MEMORY[sp]);
            pc = pc + 1;
            break;

        case 77:
            eightBitValue = readInt8(MEMORY, 1);
            sp = sp-1;
            printf("%d \n",eightBitValue);
            pc = pc + 1;
            break;
        case 78:
            sixteenTemp = readInt16(MEMORY, 2);
            sp = sp - 2;
            fullShort1 = *(uint16_t*)(&sixteenTemp);
            printf("%d \n", sixteenTemp);
            pc = pc + 1;
            break;
        case 79:
            threetwoTemp = readInt32(MEMORY, 4);
            sp = sp - 4; 
            fullInt1 = *(uint32_t*)(&threetwoTemp);
            printf("%d \n", fullInt1);
            pc = pc + 1;
            break;
        case 80:
            sixfourTemp = readInt64(MEMORY, 8);
            sp = sp - 8; 
            fullLong1 = *(uint64_t*)(&sixfourTemp);
            printf("%ld \n", fullLong1);
            pc = pc + 1;
            break;
        case 81:
            tempF = readFloat(MEMORY, 4);
            sp = sp - 4; 
            printf("%f \n", tempF);
            pc = pc + 1;
            break;
        case 82:
            tempD = readDouble(MEMORY, 8);
            sp = sp - 8; 
            printf("%lf \n", tempD);
            pc = pc + 1;
            break;
        
        //add cases
        case 83:
            mem1  = readInt8(MEMORY, 2);
            mem2  = readInt8(MEMORY, 1);
            eightBitValue  = mem1 + mem2;
            // printf("mem1: %d \n", mem1);
            // printf("mem2: %d \n", mem2);
            // printf("sum: %d \n", eightBitValue);
            //writeInt8Num(eightBitValue, MEMORY, 2);
            uint8_t data = *(uint8_t*)(&eightBitValue);
            MEMORY[sp-2] = data;
            // printf("end: %d \n", MEMORY[sp-2]);

            sp = sp - 1;
            pc = pc + 1;
            break;
            
        case 84:
            mem1_16 = readInt16(MEMORY, 4);
            mem2_16 = readInt16(MEMORY, 2);
            // printf("mem1: %d \n", mem1_16);
            // printf("mem2: %d \n", mem2_16);

            //setInt16Nums(&mem1_16,  &mem2_16, MEMORY);        
            sixteenTemp = mem1_16 + mem2_16;
            //printf("Sum: %d \n", sixteenTemp);
            writeInt16Num(sixteenTemp, MEMORY, 4);

            // printf("fir: %x \n", MEMORY[sp-4]);
            // printf("sec: %x \n", MEMORY[sp-3]);

            sp = sp - 2;
            pc = pc + 1;
            break;

        case 85:

            mem1_32 = readInt32(MEMORY, 8);
            mem2_32 = readInt32(MEMORY, 4);
            threetwoTemp = mem1_32 + mem2_32;

            // printf("mem1: %d \n", mem1_32);
            // printf("mem2: %d \n", mem2_32);
            // printf("sum: %d \n", threetwoTemp);

            writeInt32Nums(threetwoTemp, MEMORY, 8);

            // printf("Mem[sp-8]: %x \n", MEMORY[sp-8]);
            // printf("Mem[sp-7]: %x \n", MEMORY[sp-7]);
            // printf("Mem[sp-6]: %x \n", MEMORY[sp-6]);
            // printf("Mem[sp-5]: %x \n", MEMORY[sp-5]);

            sp = sp - 4; 
            pc = pc + 1;
            break;            
        case 86:

            mem1_64 = readInt64(MEMORY, 16);
            mem2_64 = readInt64(MEMORY, 8);
            sixfourTemp = mem1_64 + mem2_64;
            // printf("mem1: %ld \n", mem1_64);
            // printf("mem2: %ld \n", mem2_64);
            // printf("sum: %ld \n", sixfourTemp);
            
            writeInt64Nums(sixfourTemp, MEMORY, 16);
            
            sp = sp - 8; 
            pc = pc + 1;
            break;
        case 87:
            f1 = readFloat(MEMORY, 8);
            f2 = readFloat(MEMORY, 4);
            tempF = f1 + f2;
            // printf("mem1: %f \n", f1);
            // printf("mem2: %f \n", f2);
            // printf("sum: %f \n", tempF);

            writeFloatNum(tempF, MEMORY, 8);


            sp = sp - 4; 
            pc = pc + 1;
            break;
        case 88:
            d1 = readDouble(MEMORY, 16);
            d2 = readDouble(MEMORY, 8);
            tempD = d1 + d2;
            // printf("mem1: %f \n", d1);
            // printf("mem2: %f \n", d2);
            // printf("sum: %f \n", tempD);
            writeDoubleNum(tempD, MEMORY, 16);


            sp = sp - 8; 
            pc = pc + 1;
            break;
        
        //sub cases
        case 89:
            mem1  = readInt8(MEMORY, 2);
            mem2  = readInt8(MEMORY, 1);
            eightBitValue  = mem1 - mem2;
            // printf("mem1: %d \n", mem1);
            // printf("mem2: %d \n", mem2);
            // printf("sum: %d \n", eightBitValue);
            //writeInt8Num(eightBitValue, MEMORY, 2);
            data = *(uint8_t*)(&eightBitValue);
            MEMORY[sp-2] = data;
            // printf("end: %d \n", MEMORY[sp-2]);
 
            sp = sp - 1; 
            pc = pc + 1;
            break;
        case 90:
            mem1_16 = readInt16(MEMORY, 4);
            mem2_16 = readInt16(MEMORY, 2);
            // printf("mem1: %d \n", mem1_16);
            // printf("mem2: %d \n", mem2_16);

            //setInt16Nums(&mem1_16,  &mem2_16, MEMORY);        
            sixteenTemp = mem1_16 - mem2_16;
            //printf("Sum: %d \n", sixteenTemp);
            writeInt16Num(sixteenTemp, MEMORY, 4);

            // printf("fir: %x \n", MEMORY[sp-4]);
            // printf("sec: %x \n", MEMORY[sp-3]);            
            sp = sp - 2; 
            pc = pc + 1;
            break;

        case 91:
            mem1_32 = readInt32(MEMORY, 8);
            mem2_32 = readInt32(MEMORY, 4);
            threetwoTemp = mem1_32 - mem2_32;

            // printf("mem1: %d \n", mem1_32);
            // printf("mem2: %d \n", mem2_32);
            // printf("sum: %d \n", threetwoTemp);

            writeInt32Nums(threetwoTemp, MEMORY, 8);
            //printf("sp before sub: %d", sp);
            // printf("Mem[sp-8]: %x \n", MEMORY[sp-8]);
            // printf("Mem[sp-7]: %x \n", MEMORY[sp-7]);
            // printf("Mem[sp-6]: %x \n", MEMORY[sp-6]);
            // printf("Mem[sp-5]: %x \n", MEMORY[sp-5]);
            sp = sp - 4; 
            //printf("sp after sub: %d", sp);
            pc = pc + 1;
            break;
        case 92:
            mem1_64 = readInt64(MEMORY, 16);
            mem2_64 = readInt64(MEMORY, 8);
            sixfourTemp = mem1_64 - mem2_64;
            // printf("mem1: %ld \n", mem1_64);
            // printf("mem2: %ld \n", mem2_64);
            // printf("sum: %ld \n", sixfourTemp);
            
            writeInt64Nums(sixfourTemp, MEMORY, 16);            
            sp = sp - 8; 
            pc = pc + 1;
            break;

        case 93:
            f1 = readFloat(MEMORY, 8);
            f2 = readFloat(MEMORY, 4);
            tempF = f1 - f2;
            // printf("mem1: %f \n", f1);
            // printf("mem2: %f \n", f2);
            // printf("sum: %f \n", tempF);

            writeFloatNum(tempF, MEMORY, 8);

            sp = sp - 4; 
            pc = pc + 1;
            break;
            
        case 94:
            d1 = readDouble(MEMORY, 16);
            d2 = readDouble(MEMORY, 8);
            tempD = d1 - d2;
            // printf("mem1: %f \n", d1);
            // printf("mem2: %f \n", d2);
            // printf("sum: %f \n", tempD);
            writeDoubleNum(tempD, MEMORY, 16);
            sp = sp - 8; 
            pc = pc + 1;
            break;

        //mul cases
        case 95:
            mem1  = readInt8(MEMORY, 2);
            mem2  = readInt8(MEMORY, 1);
            eightBitValue  = mem1 * mem2;
            // printf("mem1: %d \n", mem1);
            // printf("mem2: %d \n", mem2);
            // printf("sum: %d \n", eightBitValue);
            //writeInt8Num(eightBitValue, MEMORY, 2);
            data = *(uint8_t*)(&eightBitValue);
            MEMORY[sp-2] = data;
            sp = sp - 1; 
            pc = pc + 1;
            break;
        case 96:
            mem1_16 = readInt16(MEMORY, 4);
            mem2_16 = readInt16(MEMORY, 2);
            // printf("mem1: %d \n", mem1_16);
            // printf("mem2: %d \n", mem2_16);

            //setInt16Nums(&mem1_16,  &mem2_16, MEMORY);        
            sixteenTemp = mem1_16 * mem2_16;
            //printf("Sum: %d \n", sixteenTemp);
            writeInt16Num(sixteenTemp, MEMORY, 4);
            sp = sp - 2; 
            pc = pc + 1;
            break;
        case 97:
            mem1_32 = readInt32(MEMORY, 8);
            mem2_32 = readInt32(MEMORY, 4);
            threetwoTemp = mem1_32 * mem2_32;

            // printf("mem1: %d \n", mem1_32);
            // printf("mem2: %d \n", mem2_32);
            // printf("sum: %d \n", threetwoTemp);

            writeInt32Nums(threetwoTemp, MEMORY, 8);

            sp = sp - 4; 
            pc = pc + 1;
            break;
        case 98:
            mem1_64 = readInt64(MEMORY, 16);
            mem2_64 = readInt64(MEMORY, 8);
            sixfourTemp = mem1_64 * mem2_64;
            // printf("mem1: %ld \n", mem1_64);
            // printf("mem2: %ld \n", mem2_64);
            // printf("sum: %ld \n", sixfourTemp);
            
            writeInt64Nums(sixfourTemp, MEMORY, 16);            
            sp = sp - 8; 
            pc = pc + 1;
            break;

        case 99:
            f1 = readFloat(MEMORY, 8);
            f2 = readFloat(MEMORY, 4);
            tempF = f1 * f2;
            // printf("mem1: %f \n", f1);
            // printf("mem2: %f \n", f2);
            // printf("sum: %f \n", tempF);

            writeFloatNum(tempF, MEMORY, 8);

            sp = sp - 4; 
            pc = pc + 1;
            break;
        case 100:
            d1 = readDouble(MEMORY, 16);
            d2 = readDouble(MEMORY, 8);
            tempD = d1 * d2;
            // printf("mem1: %f \n", d1);
            // printf("mem2: %f \n", d2);
            // printf("sum: %f \n", tempD);
            writeDoubleNum(tempD, MEMORY, 16); 
            sp = sp - 8; 
            pc = pc + 1;
            break;

        //div 
        case 101:
            mem1  = readInt8(MEMORY, 2);
            mem2  = readInt8(MEMORY, 1);
            if(mem2 == 0){
                myExit(2);
            }
            eightBitValue  = mem1 / mem2;
            // printf("mem1: %d \n", mem1);
            // printf("mem2: %d \n", mem2);
            // printf("sum: %d \n", eightBitValue);
            //writeInt8Num(eightBitValue, MEMORY, 2);
            data = *(uint8_t*)(&eightBitValue);
            MEMORY[sp-2] = data;
            sp = sp - 1; 
            pc = pc + 1;
            break;

        case 102:
            mem1_16 = readInt16(MEMORY, 4);
            mem2_16 = readInt16(MEMORY, 2);
            if(mem2_16 == 0){
                myExit(2);
            }
            // printf("mem1: %d \n", mem1_16);
            // printf("mem2: %d \n", mem2_16);

            //setInt16Nums(&mem1_16,  &mem2_16, MEMORY);        
            sixteenTemp = mem1_16 / mem2_16;
            //printf("Sum: %d \n", sixteenTemp);
            writeInt16Num(sixteenTemp, MEMORY, 4);
            sp = sp - 2; 
            pc = pc + 1;
            break;
        case 103:
            mem1_32 = readInt32(MEMORY, 8);
            mem2_32 = readInt32(MEMORY, 4);
            if(mem2_32 == 0){
                myExit(2);
            }
            threetwoTemp = mem1_32 / mem2_32;
            

            // printf("mem1: %d \n", mem1_32);
            // printf("mem2: %d \n", mem2_32);
            // printf("sum: %d \n", threetwoTemp);

            writeInt32Nums(threetwoTemp, MEMORY, 8); 
            sp = sp - 4; 
            pc = pc + 1;
            break;
        case 104:
            mem1_64 = readInt64(MEMORY, 16);
            mem2_64 = readInt64(MEMORY, 8);
            if(mem2_64 == 0){
                myExit(2);
            }
            sixfourTemp = mem1_64 / mem2_64;
            
            // printf("mem1: %ld \n", mem1_64);
            // printf("mem2: %ld \n", mem2_64);
            // printf("sum: %ld \n", sixfourTemp);
            
            writeInt64Nums(sixfourTemp, MEMORY, 16);  
            sp = sp - 8; 
            pc = pc + 1;
            break;
        case 105:
            f1 = readFloat(MEMORY, 8);
            f2 = readFloat(MEMORY, 4);
            if(f2 == 0){
                myExit(2);
            }
            tempF = f1 / f2;
            
            // printf("mem1: %f \n", f1);
            // printf("mem2: %f \n", f2);
            // printf("sum: %f \n", tempF);

            writeFloatNum(tempF, MEMORY, 8);
            sp = sp - 4; 
            pc = pc + 1;
            break;
        case 106:
            d1 = readDouble(MEMORY, 16);
            d2 = readDouble(MEMORY, 8);
            if(d2 == 0){
                myExit(2);
            }
            tempD = d1 / d2;
            // printf("mem1: %f \n", d1);
            // printf("mem2: %f \n", d2);
            // printf("sum: %f \n", tempD);
            writeDoubleNum(tempD, MEMORY, 16);  
            sp = sp - 8; 
            pc = pc + 1;
            break;
        
        //and cases
        case 107:
            
            MEMORY[sp - 2] = MEMORY[sp - 2] & MEMORY[sp - 1]; 
            sp = sp - 1; 
            pc = pc + 1;
            break;
        case 108:
            
            mem1_16 = readInt16(MEMORY, 4);
            mem2_16 = readInt16(MEMORY, 2);
            // printf("mem1: %d \n", mem1_16);
            // printf("mem2: %d \n", mem2_16);

            //setInt16Nums(&mem1_16,  &mem2_16, MEMORY);        
            sixteenTemp = mem1_16 & mem2_16;
            //printf("Sum: %d \n", sixteenTemp);
            writeInt16Num(sixteenTemp, MEMORY, 4);
            sp = sp - 2; 
            pc = pc + 1;
            break;
        case 109:
            fullInt1 = readUInt32(MEMORY, 8);
            fullInt2 = readUInt32(MEMORY, 4);
            fullInt3 = fullInt1 & fullInt2;
            writeUInt32Nums(fullInt3, MEMORY, 8);
            // writeInt32Nums(fullInt3, MEMORY, 8);
            sp = sp - 4; 
            pc = pc + 1;
            break;
        case 110:
            mem1_64 = readInt64(MEMORY, 16);
            mem2_64 = readInt64(MEMORY, 8);
            sixfourTemp = mem1_64 & mem2_64;
            // printf("mem1: %ld \n", mem1_64);
            // printf("mem2: %ld \n", mem2_64);
            // printf("sum: %ld \n", sixfourTemp);
            
            writeInt64Nums(sixfourTemp, MEMORY, 16);
            sp = sp - 8; 
            pc = pc + 1;
            break;

        //or cases
        case 111:
            MEMORY[sp - 2] = MEMORY[sp - 2] | MEMORY[sp - 1]; 
            sp = sp - 1; 
            pc = pc + 1;
            break;
        case 112:
            MEMORY[sp - 4] = MEMORY[sp - 4] | MEMORY[sp - 2]; 
            sp = sp - 2; 
            pc = pc + 1;
            break;
        case 113:
            fullInt1 = (uint32_t) readInt32(MEMORY, 8);
            fullInt2 = (uint32_t) readInt32(MEMORY, 4);
            fullInt3 = fullInt1 | fullInt2;
            writeInt32Nums(fullInt3, MEMORY, 8);
            sp = sp - 4; 
            pc = pc + 1;
            break;
        case 114:
            MEMORY[sp - 16] = MEMORY[sp - 16] & MEMORY[sp - 8];
            sp = sp - 8; 
            pc = pc + 1;
            break;
        

        //xor cases
        case 115:
            MEMORY[sp - 2] = MEMORY[sp - 2] ^ MEMORY[sp - 1]; 
            sp = sp - 1; 
            pc = pc + 1;
            break;
        case 116:
            fullShort1 = (MEMORY[sp-4] << 8) | MEMORY[sp-3];
            fullShort2 = (MEMORY[sp-2] << 8) | MEMORY[sp-1];
            fullShort3 = fullShort1 ^ fullShort2;
            MEMORY[sp - 4] = (fullShort3>> 8) && 0xFF;
            MEMORY[sp - 3] = (fullShort3) && 0xFF;
            sp = sp - 2; 
            pc = pc + 1;
            break;
        case 117:
            // fullInt1 = (MEMORY[sp-8] << 24) | (MEMORY[sp-7] << 16) | (MEMORY[sp-6] << 8) | MEMORY[sp-5];
            // fullInt2 = (MEMORY[sp-4] << 24) | (MEMORY[sp-3] << 16) | (MEMORY[sp-2] << 8) | MEMORY[sp-1];
            // fullInt3 = fullInt1 ^ fullInt2;
            
            // MEMORY[sp - 8] = (fullShort3 >> 24) && 0xFF;
            // MEMORY[sp - 7] = (fullShort3 >> 16) && 0xFF;
            // MEMORY[sp - 6] = (fullShort3 >> 8) && 0xFF;
            // MEMORY[sp - 5] = (fullShort3) && 0xFF; 

            fullInt1 = (uint32_t) readInt32(MEMORY, 8);
            fullInt2 = (uint32_t) readInt32(MEMORY, 4);

            fullInt3 = fullInt1 ^ fullInt2;

            writeInt32Nums(fullInt3, MEMORY, 8);
            sp = sp - 4; 
            pc = pc + 1;
            break;
        case 118:
            fullLong1 = (uint64_t) readInt64(MEMORY, 16);
            fullLong2 = (uint64_t) readInt64(MEMORY, 8);
            fullLong3 = fullLong1 ^ fullLong2;
            writeInt64Nums(fullLong3, MEMORY, 16);            
            sp = sp - 8; 
            pc = pc + 1;
            break;

        //not cases
        case 119: 
            MEMORY[sp - 1] = ~MEMORY[sp - 1]; 
            pc = pc + 1;
            break;
        case 120: //short
            
            shortByte1 = MEMORY[sp - 2];
            shortByte2 = MEMORY[sp - 1];
            fullShort1 = (shortByte1 << 8) | shortByte2;

            fullShort1 = ~fullShort1;

            MEMORY[sp-2] = (fullShort1 >> 8) & 0xFF;
            MEMORY[sp-1] = fullShort1 & 0xFF;
            pc = pc + 1;
            break;
        case 121: //int
            fullInt1 = (uint32_t) readInt32(MEMORY, 4);
            fullInt1 = ~fullInt1;

            writeInt32Nums(fullInt1, MEMORY, 4);
            pc = pc + 1;
            break;
        case 122: //long
            MEMORY[sp - 8] = ~MEMORY[sp - 8]; 
            MEMORY[sp - 7] = ~MEMORY[sp - 4]; 
            MEMORY[sp - 6] = ~MEMORY[sp - 3];
            MEMORY[sp - 5] = ~MEMORY[sp - 2];
            MEMORY[sp - 4] = ~MEMORY[sp - 4]; 
            MEMORY[sp - 3] = ~MEMORY[sp - 3];
            MEMORY[sp - 2] = ~MEMORY[sp - 2];
            MEMORY[sp - 1] = ~MEMORY[sp - 1];
            pc = pc + 1;
            break;

        case 123:
            eightBitValue = readInt8(MEMORY, 1);
            eightBitTempI = MEMORY[pc +1];
            eightBitValue >>= eightBitTempI;
            writeInt8Num(eightBitValue, MEMORY, 1);
            pc = pc + 2;
            break;
        case 124:
            sixteenTemp = readInt16(MEMORY, 2);
            eightBitTempI = MEMORY[pc +1];
            sixteenTemp >>= eightBitTempI;
            writeInt16Num(sixteenTemp, MEMORY, 2);
            pc = pc + 2;
            break;
        case 125:
            threetwoTemp = readInt32(MEMORY, 4);
            eightBitTempI = MEMORY[pc +1];
            threetwoTemp >>= eightBitTempI;
            writeInt32Nums(threetwoTemp, MEMORY, 4);
            pc = pc + 2;
            break;
        case 126:
            sixfourTemp = readInt64(MEMORY, 8);
            eightBitTempI = MEMORY[pc +1];
            sixfourTemp >>= eightBitTempI;
            writeInt64Nums(sixfourTemp, MEMORY, 8); 
            pc = pc + 2;
            break;

        //shift left way
        case 127:
            eightBitValue = readInt8(MEMORY, 1);
            eightBitTempI = MEMORY[pc +1];
            eightBitValue <<= eightBitTempI;
            writeInt8Num(eightBitValue, MEMORY, 1);
            pc = pc + 2;
            break;
        case 128:
            sixteenTemp = readInt16(MEMORY, 2);
            eightBitTempI = MEMORY[pc +1];
            sixteenTemp <<= eightBitTempI;
            writeInt16Num(sixteenTemp, MEMORY, 2);
            pc = pc + 2;
            break;
        case 129:
            threetwoTemp = readInt32(MEMORY, 4);
            eightBitTempI = MEMORY[pc +1];
            threetwoTemp <<= eightBitTempI;
            writeInt32Nums(threetwoTemp, MEMORY, 4);
            pc = pc + 2;
            break;
        case 130:
            sixfourTemp = readInt64(MEMORY, 8);
            eightBitTempI = MEMORY[pc +1];
            sixfourTemp <<= eightBitTempI;
            writeInt64Nums(sixfourTemp, MEMORY, 8); 
            pc = pc + 2;
            break;

        //jmp adress
        case 131:
            address = readAddress3Bytes(MEMORY);
            verifyAddress(address);
            pc = address;
            break;

        case 132:
            eightBitValue = MEMORY[pc + 1];
            pc = pc + (int)eightBitValue;
            break;
        case 133: //jind
            pc = readUInt32(MEMORY, 4);
            break;
            //jz:
        case 134:
            address = readAddress3Bytes(MEMORY);
            verifyAddress(address);
            pc = (readInt32(MEMORY, 4) == 0)? address : pc + 4;
            break;
        case 135:   
            address = readAddress3Bytes(MEMORY);
            verifyAddress(address);
            pc = readInt32(MEMORY,4) != 0? address : pc + 4;
            break;

        case 136:
            address = readAddress3Bytes(MEMORY);
            verifyAddress(address);
            pc = readInt32(MEMORY, 4) > 0?address : pc+4;
            break;
        case 137:
            address = readAddress3Bytes(MEMORY);
            verifyAddress(address);
            // fullInt1 = readUInt32(MEMORY, 4);
            pc = readInt32(MEMORY,4) < 0? address : pc + 4;
            break;

        case 138:
            address = readAddress3Bytes(MEMORY);
            verifyAddress(address);
            pc = readInt32(MEMORY,4) >= 0? address : pc + 4;
            break;

        case 139:
            address = readAddress3Bytes(MEMORY);
            verifyAddress(address);
            pc = readInt32(MEMORY, 4) <= 0? address : pc + 4;
            break;
        case 140:
            address = readAddress3Bytes(MEMORY);
            verifyAddress(address);
            sp = sp + 4; 
            writeInt32Nums((pc + 4), MEMORY, 4);
            //MEMORY[sp] = pc + 4; 
            pc = address;
            break;
        case 141:
            pc = readUInt32(MEMORY, 4);
            sp = sp - 4;
            break;
        case 142:
            myExit(0);
        default:
            myExit(1);
    }
    
}   


uint32_t readAddress3Bytes(uint8_t * MEMORY){
    uint32_t data;
    data = ((uint32_t)MEMORY[pc+1] << 16) | ((uint32_t)MEMORY[pc+2] << 8) | MEMORY[pc+3];
    return data;
}

void writeAddress(uint32_t address, uint8_t * MEMORY){
    
    for(int i = 1; i < 4; i++){
        uint8_t byte = 0;
        byte |= ((address >> (4-1-i)*8) && 0xFF);
        MEMORY[sp + i] =  byte;
        // printf("hai: %x \n",MEMORY[(sp-loc) + i] );
    }
}

// void get16BitValue(uint8_t * MEMORY, uint16_t* val){
//     int8_t byte1 = MEMORY[pc + 1];
//     int8_t byte2 = MEMORY[pc + 2];
//     *val = (byte1 << 8) | byte2;
// }
int8_t to8From16(int16_t num){
    if (num < INT8_MIN) {
        return INT8_MIN; 
    } else if (num > INT8_MAX) {
        return INT8_MAX; 
    } else {
        return (int8_t)num; 
    }

}

int8_t to8From32(int32_t num){
    if (num < INT8_MIN) {
        return INT8_MIN; 
    } else if (num > INT8_MAX) {
        return INT8_MAX; 
    } else {
        return (int8_t)num; 
    }
}

int16_t to16(int8_t num){
    return (int16_t) num;
}

int16_t to16From32(int32_t num){
    if (num < INT16_MIN) {
        return INT16_MIN; 
    } else if (num > INT16_MAX) {
        return INT16_MAX; 
    } else {
        return (int16_t)num; 
    }
}

int32_t to32(int8_t num){
    return (int32_t) num;
}

int32_t to32From16(int16_t num){
    return (int32_t) num;
}

int64_t to64(int8_t num){
    return (int64_t) num;
}

int64_t to64From32(int32_t num){
    return (int64_t) num;
}

int64_t to64From16(int16_t num){
    return (int64_t) num;
}
 
float toFloat(int8_t num){
    return (float) num;
}

float toFloatFrom16(int16_t num){
    return (float) num;
}

float toFloatFrom32(int32_t num){
    return (float)num;
}

double toDouble(int8_t num){
    return (double) num;
}

double toDoubleFrom16(int16_t num){
    return (double) num;
}

double toDoubleFrom32(int32_t num){
    return (double)num;
}

int8_t adjustForSign(int8_t num){
    if(num >= 0){
        return num;
    }
    else
    {
        int8_t ret = 256 + num;
        return ret;
    }
}

int16_t getInt16Num(uint8_t *MEMORY, int starting){
    // int16_t byte1;
    // byte1 = ((MEMORY[sp-starting]) << 8) | (uint8_t)(MEMORY[sp-starting-1]);
    // return byte1;

    int16_t byte1 = 0;
    for(int i = 0; i < 2; i++){
        byte1 <<= 8;
        byte1 |= MEMORY[(starting)+i];
    }
    return byte1;
}

int8_t to8From64(int64_t num){
    if (num > INT8_MAX) {
        return INT8_MAX;
    } else if (num < INT8_MIN) {
        return INT8_MIN;
    } else {
        return (int8_t)num;
    }
}
int16_t to16From64(int64_t num){
    if (num > INT16_MAX) {
        return INT16_MAX;
    } else if (num < INT16_MIN) {
        return INT16_MIN;
    } else {
        return (int16_t)num;
    }
}
int32_t to32From64(int64_t num){
    if (num > INT32_MAX) {
        return INT32_MAX;
    } else if (num < INT32_MIN) {
        return INT32_MIN;
    } else {
        return (int32_t)num;
    }
}
float toFloatFrom64(int64_t num){
    return (float)num;
}
double toDoubleFrom64(int64_t num){
    return (double)num;
}


void setInt16Nums(int16_t *mem1_16, int16_t* mem2_16, int8_t * MEMORY){
    int16_t byte1;
    byte1 = (adjustForSign(MEMORY[sp-4]) << 8) | adjustForSign(MEMORY[sp-3]);
    *mem1_16 = byte1;
    int16_t byte2;
    byte2 = (adjustForSign(MEMORY[sp-2] << 8)) | adjustForSign(MEMORY[sp-1]);
    *mem2_16 = byte2;
}

void setInt32Nums(int32_t *mem1_32, int32_t *mem2_32, int8_t * MEMORY){
    int16_t byte1, byte2, byte3, byte4;
    printf("mem1: %x \n", MEMORY[sp-8]);
    printf("mem1: %x\n", MEMORY[sp-7]);
    printf("mem1: %x\n", MEMORY[sp-6]);
    printf("mem1: %x\n", MEMORY[sp-5]);


    byte1 = (MEMORY[sp-8] << 8) | (uint8_t)MEMORY[sp-7];
    byte2 = (MEMORY[sp-6] << 8) | (uint8_t)MEMORY[sp-5];

    *mem1_32 = ((int32_t)byte1 << 16) | (int32_t)byte2;

    byte3 = (MEMORY[sp-4] << 8) | (uint8_t)MEMORY[sp-3];
    byte4 = (MEMORY[sp-2] << 8) | (uint8_t)MEMORY[sp-1];

    *mem2_32 = ((int32_t)byte3 << 16) | (int32_t)byte4;
}

void writeInt8Num(int8_t num, uint8_t * MEMORY, int loc){
    uint8_t data = *(uint8_t*)(&num);
    //printf("data: %d \n", data);
    uint8_t byte = 0;
    byte |= (data >> 0);
    MEMORY[(sp-loc)] =  byte;
    // printf("hai: %x \n",MEMORY[(sp-loc) + i] );
}

void writeInt8Address(int8_t num, uint8_t * MEMORY, uint32_t addr){
    uint8_t data = *(uint8_t*)(&num);
    //printf("data: %d \n", data);
    uint8_t byte = 0;
    byte |= (data >> 0);
    MEMORY[addr] =  byte;
}


void writeInt16Num(int16_t num, uint8_t * MEMORY, int loc){
   // printf("num: %d \n", num);
    uint16_t data = *(uint16_t*)(&num);
    // printf("data: %d \n", data);
    for(int i = 0; i < 2; i++){
        uint8_t byte = 0;
        byte |= (data >> (2-1-i)*8);
        MEMORY[(sp-loc) + i] =  byte;
       // printf("hai: %x \n",MEMORY[(sp-loc) + i] );
    }
}

void writeInt16Address(int16_t num, uint8_t * MEMORY, uint32_t addr){
   // printf("num: %d \n", num);
    uint16_t data = *(uint16_t*)(&num);
    // printf("data: %d \n", data);
    for(int i = 0; i < 2; i++){
        uint8_t byte = 0;
        byte |= (data >> (2-1-i)*8);
        MEMORY[(addr) + i] =  byte;
       // printf("hai: %x \n",MEMORY[(sp-loc) + i] );
    }
}

void writeInt32Nums(int32_t num, uint8_t * MEMORY, int loc){
    uint32_t data = *(uint32_t*)(&num);
    // printf("data: %d \n", data);
    for(int i = 0; i < 4; i++){
        uint8_t byte = 0;
        byte |= (data >> (4-1-i)*8);
        MEMORY[(sp-loc) + i] =  byte;
        // printf("hai: %x \n",MEMORY[(sp-loc) + i] );
    }
}

void writeInt32Address(int32_t num, uint8_t * MEMORY, uint32_t addr){
    uint32_t data = *(uint32_t*)(&num);
    for(int i = 0; i < 4; i++){
        uint8_t byte = 0;
        byte |= (data >> (4-1-i)*8);
        MEMORY[(addr) + i] =  byte;
    }
}


void writeInt64Nums(int64_t num, uint8_t * MEMORY, int loc){
    uint64_t data = *(uint64_t*)(&num);
    // printf("data: %d \n", data);
    for(int i = 0; i < 8; i++){
        uint8_t byte = 0;
        byte |= (data >> (7-i)*8);
        MEMORY[(sp-loc) + i] =  byte;
        // printf("hai: %x \n",MEMORY[(sp-loc) + i] );
    }
}

void writeInt64Address(int64_t num, uint8_t * MEMORY, uint32_t addr){
    uint64_t data = *(uint64_t*)(&num);
    for(int i = 0; i < 8; i++){
        uint8_t byte = 0;
        byte |= (data >> (7-i)*8);
        MEMORY[(addr) + i] =  byte;
    }
}



void writeFloatNum(float tempF, uint8_t * MEMORY, int loc){
    uint32_t data = *(uint32_t*)(&tempF);
    for(int i = 0; i < 4; i++){
        uint8_t byte = 0;
        byte |= (data >> (4-1-i)*8);
        MEMORY[(sp-loc) + i] =  byte;
       // printf("hai: %x \n",MEMORY[(sp-loc) + i] );
    }
}

void writeFloatAddress(float tempF, uint8_t * MEMORY, uint32_t addr){
    uint32_t data = *(uint32_t*)(&tempF);
    for(int i = 0; i < 4; i++){
        uint8_t byte = 0;
        byte |= (data >> (4-1-i)*8);
        MEMORY[(addr) + i] =  byte;
       // printf("hai: %x \n",MEMORY[(sp-loc) + i] );
    }
}

void writeDoubleNum(double tempD, uint8_t * MEMORY, int loc){
    uint64_t data = *(uint64_t*)(&tempD);
    for(int i = 0; i < 8; i++){
        uint8_t byte = 0;
        byte |= (data >> (7-i)*8);
        MEMORY[(sp-loc) + i] =  byte;
        // printf("hai: %x \n",MEMORY[(sp-loc) + i] );
    }
}

void writeDoubleAddress(double tempD, uint8_t * MEMORY, uint32_t addr){
    uint64_t data = *(uint64_t*)(&tempD);
    for(int i = 0; i < 8; i++){
        uint8_t byte = 0;
        byte |= (data >> (7-i)*8);
        MEMORY[(addr) + i] =  byte;
        // printf("hai: %x \n",MEMORY[(sp-loc) + i] );
    }
}

void writeUInt32Nums(uint32_t num, uint8_t * MEMORY, int loc){
    uint32_t data = num;
    for(int i = 0; i < 4; i++){
        uint8_t byte = 0;
        byte |= (data >> (4-1-i)*8);
        MEMORY[(sp-loc) + i] =  byte;
    }

}

int8_t readInt8(uint8_t * MEMORY, int loc){
    int8_t byte = 0;
    uint8_t bites[1];
    for(int i = 0; i < 1; i++){
        bites[i] = (uint8_t)MEMORY[(sp-loc)+i];
        //printf("bite: %x at %d \n", bites[i], i);
    }
    for(int i = 0; i < 1; i++){
        byte |= (bites[i] << 0);
    }
    return byte;
}

int8_t readInt8Address(uint8_t * MEMORY, uint32_t addr){
    int8_t byte = 0;
    uint8_t bites[1];
    for(int i = 0; i < 1; i++){
        bites[i] = (uint8_t)MEMORY[(addr)+i];
        //printf("bite: %x at %d \n", bites[i], i);
    }
    for(int i = 0; i < 1; i++){
        byte |= (bites[i] << 0);
    }
    return byte;
}


int16_t readInt16(uint8_t *MEMORY, int loc){
    int16_t byte = 0;
    uint8_t bites[2];
    for(int i = 0; i < 2; i++){
        bites[i] = (uint8_t)MEMORY[(sp-loc)+i];
        //printf("bite: %x at %d \n", bites[i], i);
    }
    for(int i = 0; i < 2; i++){
        byte |= (bites[i] << (1-i)*8);
    }
    return byte;
}

int16_t readInt16Address(uint8_t *MEMORY, uint32_t addr){
    int16_t byte = 0;
    uint8_t bites[2];
    for(int i = 0; i < 2; i++){
        bites[i] = (uint8_t)MEMORY[(addr)+i];
        //printf("bite: %x at %d \n", bites[i], i);
    }
    for(int i = 0; i < 2; i++){
        byte |= (bites[i] << (1-i)*8);
    }
    return byte;
}


int32_t readInt32(uint8_t *MEMORY, int loc){
    int32_t byte = 0;
    uint8_t bites[4];
    for(int i = 0; i < 4; i++){
        bites[i] = (uint8_t)MEMORY[(sp-loc)+i];
        // printf("bite: %d at %d \n", bites[i], i);
    }
    for(int i = 0; i < 4; i++){
        byte |= (bites[i] << (3-i)*8);
    }
    return byte;
}

int32_t readInt32Address(uint8_t *MEMORY, uint32_t addr){
    int32_t byte = 0;
    uint8_t bites[4];
    for(int i = 0; i < 4; i++){
        bites[i] = (uint8_t)MEMORY[(addr)+i];
        // printf("bite: %d at %d \n", bites[i], i);
    }
    for(int i = 0; i < 4; i++){
        byte |= (bites[i] << (3-i)*8);
    }
    return byte;
}

int64_t readInt64(uint8_t *MEMORY, int loc){
    int64_t byte = 0;
    uint8_t bites[8];
    for(int i = 0; i < 8; i++){
        bites[i] = (uint8_t)MEMORY[(sp-loc)+i];
        // printf("bite: %d at %d \n", bites[i], i);
    }
    // for(int i = 0; i < 8; i++){
    //     byte |= (bites[i] << (7-i)*8);
    // }
    byte = ((uint64_t)bites[0] << 56) | ((uint64_t)bites[1] << 48) | ((uint64_t)bites[2] << 40) | ((uint64_t)bites[3] << 32) 
    | ((uint64_t)bites[4] << 24) | ((uint64_t)bites[5] << 16) | ((uint64_t)bites[6] << 8) | ((uint64_t)bites[7] << 0);

    //printf("full byte: %lx \n", byte);
    return byte;
}

int64_t readInt64Address(uint8_t *MEMORY, uint32_t addr){
    int64_t byte = 0;
    uint8_t bites[8];
    for(int i = 0; i < 8; i++){
        bites[i] = (uint8_t)MEMORY[(addr)+i];
    }
    byte = ((uint64_t)bites[0] << 56) | ((uint64_t)bites[1] << 48) | ((uint64_t)bites[2] << 40) | ((uint64_t)bites[3] << 32) 
    | ((uint64_t)bites[4] << 24) | ((uint64_t)bites[5] << 16) | ((uint64_t)bites[6] << 8) | ((uint64_t)bites[7] << 0);
    return byte;
}

float readFloat(uint8_t *MEMORY, int loc){
    uint32_t byte = 0;
    uint8_t bites[4];
    for(int i = 0; i < 4; i++){
        bites[i] = (uint8_t)MEMORY[(sp-loc)+i];
       // printf("bite: %d at %d \n", bites[i], i);
    }
    for(int i = 0; i < 4; i++){
        byte |= (bites[i] << (3-i)*8);
    }
    return *(float*)(&byte);
}


float readFloatAddress(uint8_t *MEMORY, uint32_t addr){
    uint32_t byte = 0;
    uint8_t bites[4];
    for(int i = 0; i < 4; i++){
        bites[i] = (uint8_t)MEMORY[(addr)+i];
       // printf("bite: %d at %d \n", bites[i], i);
    }
    for(int i = 0; i < 4; i++){
        byte |= (bites[i] << (3-i)*8);
    }
    return *(float*)(&byte);
}

double readDouble(uint8_t *MEMORY, int loc){
    int64_t byte = 0;
    uint8_t bites[8];
    for(int i = 0; i < 8; i++){
        bites[i] = (uint8_t)MEMORY[(sp-loc)+i];
        // printf("bite: %d at %d \n", bites[i], i);
    }
    // for(int i = 0; i < 8; i++){
    //     byte |= (bites[i] << (7-i)*8);
    // }
    byte = ((uint64_t)bites[0] << 56) | ((uint64_t)bites[1] << 48) | ((uint64_t)bites[2] << 40) | ((uint64_t)bites[3] << 32) 
    | ((uint64_t)bites[4] << 24) | ((uint64_t)bites[5] << 16) | ((uint64_t)bites[6] << 8) | ((uint64_t)bites[7] << 0);

    //printf("full byte: %lx \n", byte);
    //printf("full byte: %lx \n", byte);
    return *(double*)(&byte);
}


double readDoubleAddress(uint8_t *MEMORY, uint32_t addr){
    int64_t byte = 0;
    uint8_t bites[8];
    for(int i = 0; i < 8; i++){
        bites[i] = (uint8_t)MEMORY[(addr)+i];
    }
    byte = ((uint64_t)bites[0] << 56) | ((uint64_t)bites[1] << 48) | ((uint64_t)bites[2] << 40) | ((uint64_t)bites[3] << 32) 
    | ((uint64_t)bites[4] << 24) | ((uint64_t)bites[5] << 16) | ((uint64_t)bites[6] << 8) | ((uint64_t)bites[7] << 0);
    return *(double*)(&byte);
}


uint8_t readUInt8(uint8_t * MEMORY, int loc){
    uint8_t byte = 0;
    uint8_t bites[1];
    for(int i = 0; i < 1; i++){
        bites[i] = (uint8_t)MEMORY[(sp-loc)+i];
        //printf("bite: %x at %d \n", bites[i], i);
    }
    for(int i = 0; i < 1; i++){
        byte |= (bites[i] << 0);
    }
    return byte;
}


uint32_t readUInt32(uint8_t *MEMORY, int loc){
    uint32_t byte = 0;
    uint8_t bites[4];
    for(int i = 0; i < 4; i++){
        bites[i] = (uint8_t)MEMORY[(sp-loc)+i];
        //printf("bite: %d at %d \n", bites[i], i);
    }
    for(int i = 0; i < 4; i++){
        byte |= (bites[i] << (3-i)*8);
    }
    return byte;
}

uint64_t readUInt64(uint8_t *MEMORY, int loc){
    uint64_t byte = 0;
    uint8_t bites[8];
    for(int i = 0; i < 8; i++){
        bites[i] = (uint8_t)MEMORY[(sp-loc)+i];
        //printf("bite: %d at %d \n", bites[i], i);
    }
    for(int i = 0; i < 4; i++){
        byte |= (bites[i] << (7-i)*8);
    }
    return byte;
}

uint8_t readUInt8PC(uint8_t * MEMORY){
    uint8_t byte = 0;
    uint8_t bites[1];
    for(int i = 0; i < 1; i++){
        bites[i] = (uint8_t)MEMORY[(pc+1)+i];
        //printf("bite: %x at %d \n", bites[i], i);
    }
    for(int i = 0; i < 1; i++){
        byte |= (bites[i] << 0);
    }
    return byte;
}


uint16_t readUInt16PC(uint8_t *MEMORY){
    uint16_t byte = 0;
    uint8_t bites[2];
    for(int i = 0; i < 2; i++){
        bites[i] = (uint8_t)MEMORY[(pc+1+i)];
        //printf("bite: %x at %d \n", bites[i], i);
    }
    for(int i = 0; i < 2; i++){
        byte |= (bites[i] << (1-i)*8);
    }
    return byte;
}


uint32_t readUInt32PC(uint8_t *MEMORY){
    uint32_t byte = 0;
    uint8_t bites[4];
    for(int i = 0; i < 4; i++){
        bites[i] = (uint8_t)MEMORY[(pc+1)+i];
        //printf("bite: %d at %d \n", bites[i], i);
    }
    for(int i = 0; i < 4; i++){
        byte |= (bites[i] << (3-i)*8);
    }
    return byte;
}

uint64_t readUInt64PC(uint8_t *MEMORY){
    uint64_t byte = 0;
    uint8_t bites[8];
    for(int i = 0; i < 8; i++){
        bites[i] = (uint8_t)MEMORY[(pc+1)+i];
        //printf("bite: %d at %d \n", bites[i], i);
    }
    for(int i = 0; i < 8; i++){
        byte |= (bites[i] << (7-i)*8);
    }
    return byte;
}

float readFloatPC(uint8_t *MEMORY){
    uint32_t byte = 0;
    uint8_t bites[4];
    for(int i = 0; i < 4; i++){
        bites[i] = (uint8_t)MEMORY[(pc+1)+i];
    }
    for(int i = 0; i < 4; i++){
        byte |= (bites[i] << (3-i)*8);
    }
    return *(float*)(&byte);
}

double readDoublePC(uint8_t *MEMORY){
    int64_t byte = 0;
    uint8_t bites[8];
    for(int i = 0; i < 8; i++){
        bites[i] = (uint8_t)MEMORY[(pc+1)+i];
    }
    byte = ((uint64_t)bites[0] << 56) | ((uint64_t)bites[1] << 48) | ((uint64_t)bites[2] << 40) | ((uint64_t)bites[3] << 32) 
    | ((uint64_t)bites[4] << 24) | ((uint64_t)bites[5] << 16) | ((uint64_t)bites[6] << 8) | ((uint64_t)bites[7] << 0);
    return *(double*)(&byte);
}


int8_t to8FromFloat(float num){
    if (num > INT8_MAX) {
        return INT8_MAX;
    } else if (num < INT8_MIN) {
        return INT8_MIN;
    } else {
        return (int8_t)num;
    }
}
int16_t to16FromFloat(float num){
    if (num > INT16_MAX) {
        return INT16_MAX;
    } else if (num < INT16_MIN) {
        return INT16_MIN;
    } else {
        return (int16_t)num;
    }
}
int32_t to32FromFloat(float num){
    if (num > INT32_MAX) {
        return INT32_MAX;
    } else if (num < INT32_MIN) {
        return INT32_MIN;
    } else {
        return (int32_t)num;
    }
}
int64_t to64FromFloat(float num){
    if (num > INT64_MAX) {
        return INT64_MAX;
    } else if (num < INT64_MIN) {
        return INT64_MIN;
    } else {
        return (int64_t)num;
    }
}
double toDoubleFromFloat(float num){
    return (double)num;
}

int8_t to8FromDouble(int64_t num){
    if (num > INT8_MAX) {
        return INT8_MAX;
    } else if (num < INT8_MIN) {
        return INT8_MIN;
    } else {
        return (int8_t)num;
    }
}
int16_t to16FromDouble(int64_t num){
    if (num > INT16_MAX) {
        return INT16_MAX;
    } else if (num < INT16_MIN) {
        return INT16_MIN;
    } else {
        return (int16_t)num;
    }
}
int32_t to32FromDouble(int64_t num){
    if (num > INT32_MAX) {
        return INT32_MAX;
    } else if (num < INT32_MIN) {
        return INT32_MIN;
    } else {
        return (int32_t)num;
    }
}
int64_t to64FromDouble(int64_t num){
    if (num > (double)INT64_MAX) {
        return INT64_MAX;
    } else if (num < (double)INT64_MIN) {
        return INT64_MIN;
    } else {
        return (int64_t)num;
    }
}
float toFloatFromDouble(int64_t num){
    return (float)num;
}



char getCharInput(){
    char buffer[100];
    char ch;
    scanf("%s", buffer);
    if(strlen(buffer) > 1){
        myExit(5);
    }
    if(isdigit(buffer[0])){
        myExit(10);
    }
    ch = buffer[0];
    return ch;
}

int getByteInput(){
    char buffer[100];
    scanf("%s", buffer);
    for(int i = 0; i < strlen(buffer); i++){
        if(!isdigit(buffer[0])){
            myExit(1);
        }
    }
    int byte = atoi(buffer);
    if(byte < INT8_MIN || byte > INT8_MAX){
        myExit(1);
    }
    return byte;
}
int getShortInput(){
    char buffer[100];
    scanf("%s", buffer);
    for(int i = 0; i < strlen(buffer); i++){
        if(!isdigit(buffer[0])){
            myExit(1);
        }
    }
    int byte = atoi(buffer);
    if(byte < INT16_MIN || byte > INT16_MAX){
        myExit(1);
    }
    return byte;
}

int getIntInput(){
    char buffer[100];
    scanf("%s", buffer);
    for(int i = 0; i < strlen(buffer); i++){
        if(!isdigit(buffer[0])){
            myExit(1);
        }
    }
    int byte = atoi(buffer);
    if(byte < INT32_MIN || byte > INT32_MAX){
        myExit(1);
    }
    return byte;
}

long getLongInput(){
    char buffer[100];
    scanf("%s", buffer);
    for(int i = 0; i < strlen(buffer); i++){
        if(!isdigit(buffer[0])){
            myExit(1);
        }
    }
    long byte = strtol(buffer, NULL, 10);
    if(byte < INT64_MIN || byte > INT64_MAX){
        myExit(1);
    }
    return byte;
}

float getFloatInput(){
    char buffer[100];
    scanf("%s", buffer);
    for(int i = 0; i < strlen(buffer); i++){
         if(!isdigit(buffer[0])){
            if( buffer[0] != '.'){
                myExit(1);
            }
        }
    }
    float byte = strtof(buffer, NULL);
    if(byte < FLT_MIN || byte > FLT_MAX){
        myExit(1);
    }
    return byte;
}

double getDoubleInput(){
    char buffer[100];
    scanf("%s", buffer);
    for(int i = 0; i < strlen(buffer); i++){
        if(!isdigit(buffer[0])){
            if( buffer[0] != '.'){
                myExit(1);
            }
        }
    }
    double byte = strtof(buffer, NULL);
    if(byte < DBL_MIN || byte > DBL_MAX){
        myExit(1);
    }
    return byte;
}

int myExit(int x){
    if(x == 0){
        exit(0);
    }
    fprintf(stderr, "Simulation error \n");
    exit(x);
}

void verifyAddress(uint32_t address){
    if(address < 0 || address > MEM_SIZE){
        myExit(1);
    }
}




// void get32BitValue(){
    
// }

// void get64BitValue(){
    
// }

// void getFloatValue(){
    
// }

// void getDoubleValue(){
    
// }


