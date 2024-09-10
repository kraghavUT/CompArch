#include <stdio.h>
#include <stdbool.h>
#include <regex.h>
#include <string.h>
#include <stdint.h>
#include<ctype.h>
#include <stdlib.h>
#include <float.h>

char * removeFirstChar(char *str);
int findInstructionSize(char * str);
void resetGlobals();
int opcodeFromInstruction(char *token);
int verifyRegex(char *token, char * regexPattern);

void get8BitOperand(char *token, int * num);
void get16BitOperand(char *token, int * num);
void get32BitOperand(char *token, long long * num);
void get64BitOperand(char *token, long long * num);
void get64BitValue(char * token, double *value);
void get32BitValue(char * token, float* val);
void get24BitAddress(char *token, int *num);
void get8BitItem(char *token, int * num);
void getGeneral24BitAddress(char *token, int * num);
void writeOPToFile(int op, FILE * file);
void write8BitOperand(int operand, FILE * file);
void write16BitOperand(int operand, FILE * file);
void write32BitOperand(int operand, FILE * file);
void write64BitOperand(long operandL, FILE * file);
void write24BitAddress(int address, FILE * file);
void writeFloatToFile(float number, FILE * file);
void writeDoubleToFile(double number, FILE * file);

void writeCodeDirHeader(FILE * file);
void writeDataDirHeader(int counterCode, FILE * file);

int verifyLabelRegex(char * str);
int myExit(int x, char* fName);

int getByteNumber(char * str);
int getShortNumber(char * str);
int getIntNumber(char * str);
long getLongNumber(char * str);
float getFloatNumber(char * str);
double getDoubleNumber(char * str);


bool passedByteDirective = false;
bool passedASCIIDirective = false;
bool passedShortDirective = false;
bool passedIntDirective = false;
bool passedLongDirective = false;
bool passedFloatDirective = false;
bool passedDoubleDirective = false;

bool failed = false;
char outFileName[100];

int currLineNum = 0;

int main(int argc, char** argv){
    FILE *file;
    char buffer[100];
    file = fopen(argv[1], "r");
    if (file == NULL) {
         // printf("Error opening file.\n");
          return 1;
    }

    struct Label{
    char  name[50];
    int   address;
    int type;
    }label;

    int countLabels = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if(buffer[0] == ':'){
          countLabels++;
        }
    }
    //printf("Num labels: %d \n", countLabels);
    rewind(file);

    
    FILE *f_out;    
    char writeFileName[300];
    strcpy(writeFileName, argv[1]);
    int len = strlen(writeFileName);
    writeFileName[len] = 'o';
    writeFileName[len + 1] = '\0';
    strcpy(outFileName, writeFileName);

    struct Label allLabels[countLabels];
    int labelIndex = 0;


    //second pass
    bool passedDataDirective = false;
    bool passedCodeDirective  = false;
    int dataMemCounter = 0x10000;
    int codeMemCounter = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL){
        currLineNum++;
        //printf("Buffer: %s \n", buffer);
        if(strcmp(buffer, ".code\n") == 0){
            passedCodeDirective = true;
            passedDataDirective = false;
        }
        else if(strcmp(buffer, ".data\n") == 0){
            passedDataDirective = true;
            passedCodeDirective = false;
        }

        else if(strcmp(buffer, ".byte\n") == 0){
            passedByteDirective = true;
            continue;
        }
        else if(strcmp(buffer, ".ascii\n") == 0){
            passedASCIIDirective = true;
            continue;
        }
        else if(strcmp(buffer, ".short\n") == 0){
            passedShortDirective = true;
            continue;
        }
        else if(strcmp(buffer, ".int\n") == 0){
            passedIntDirective = true;
            continue;
        }
        else if(strcmp(buffer, ".long\n") == 0){
            passedLongDirective = true;
            continue;
        }
        else if(strcmp(buffer, ".float\n") == 0){
            passedFloatDirective = true;
            continue;
        }
        else if(strcmp(buffer, ".double\n") == 0){
            passedDoubleDirective = true;
            continue;
        }

        else if(buffer[0] == ';'){
            //Comment, ignore and continue to next pass
            continue;
        }
        else if(buffer[0] == '\t' && passedCodeDirective){
            //instruction
            char *token = strtok(buffer, " ");
            //printf("instr %s \n", token);
            int instructionSize = findInstructionSize(token);
            codeMemCounter += instructionSize;
            continue;
        }
        else if(buffer[0] == '\t' && passedDataDirective){
            //data
            if(passedByteDirective){
                dataMemCounter += 1; //8 bits
            }
            else if(passedASCIIDirective){
                dataMemCounter += 1; //8 bit ascii as per spec
            }
            else if(passedShortDirective){
                dataMemCounter += 2;
            }
            else if(passedIntDirective){
                dataMemCounter += 4;
            }
            else if(passedLongDirective){
                dataMemCounter += 8;
            }
            else if(passedFloatDirective){
                dataMemCounter += 4;
            }
            else if(passedDoubleDirective){
                dataMemCounter += 8;
            }
            // else     return error for malformat or is there a default size?
            //     dataMemCounter += 
        }
        else if(buffer[0] == ':'){
            //label found
            regex_t testReg;
            int compReg = regcomp(&testReg,":[a-zA-Z0-9_-]+", REG_EXTENDED);
            int execReg = regexec(&testReg, buffer, 0, NULL, 0);
            int length = sizeof(allLabels) / sizeof(allLabels[0]);
            if(execReg == REG_NOMATCH){
                myExit(1, outFileName);
            }
            bool checkIfSameName = false;
            for(int i = 0; i < length; i++){
                if(strcmp(allLabels[i].name, buffer) == 0){
                    checkIfSameName = true;
                    //stderr();
                }
            }
            if(checkIfSameName){
                myExit(1, outFileName);
            }
           // printf("made it here \n");
           
            strcpy(allLabels[labelIndex].name, buffer);
            if(passedDataDirective){
                allLabels[labelIndex].address = dataMemCounter;

                if(passedByteDirective){
                    allLabels[labelIndex].type = 1;
                }
                else if(passedASCIIDirective){
                    allLabels[labelIndex].type = 1; 
                }
                else if(passedShortDirective){
                    allLabels[labelIndex].type = 2;
                }
                else if(passedIntDirective){
                    allLabels[labelIndex].type = 4;
                }
                else if(passedLongDirective){
                    allLabels[labelIndex].type = 8;
                }
                else if(passedFloatDirective){
                    allLabels[labelIndex].type = 4;
                }
                else if(passedDoubleDirective){
                    allLabels[labelIndex].type = 8;
                }
             //   printf("made it here");
            }
            else if(passedCodeDirective){
              //  printf("made it here");
                allLabels[labelIndex].address = codeMemCounter;
                allLabels[labelIndex].type = 0;
            }

            labelIndex++;
        }   
    }
    // printf("Code mem %d \n", codeMemCounter);
    // printf("Data mem %d \n", dataMemCounter);
    // printf("First label: %s" , allLabels[0].name);
    // printf("First address: %d \n" , allLabels[0].address);
    // printf("First type: %d \n" , allLabels[0].type);

    // printf("second label: %s" , allLabels[1].name);
    // printf("second address: %d \n" , allLabels[1].address);
    // printf("second type: %d \n" , allLabels[1].type);
    rewind(file);
    resetGlobals();



    // printf("write file: %s \n", writeFileName);
    f_out = fopen(writeFileName, "wb");
    if (f_out == NULL) {
        perror("Failed to create file");
        return 1;
    }
    writeCodeDirHeader(f_out);
    writeDataDirHeader(codeMemCounter, f_out);

    bool enterTheCodeDir = false;
    
    while (fgets(buffer, sizeof(buffer), file) != NULL){
        currLineNum++;
        if(buffer[0] == ';'){
            continue;
        } 
        if(strcmp(buffer, ".code\n") == 0){
            enterTheCodeDir = true;
            continue;
        }
        if(strcmp(buffer, ".data\n") == 0){
            enterTheCodeDir = false;
            continue;
        }
        if(enterTheCodeDir && buffer[0] == '\t'){
            //printf("current line: %s", buffer);
            int OPCODE = opcodeFromInstruction(buffer);
            //printf("opcode: %d \n", OPCODE);
            char temp[100];
            strcpy(temp, buffer);
            if(OPCODE == -1){
                //regex failed; could be a label
                //extract instr
                char * token = strtok(buffer, " ");
                // printf("pre tok: %s \n", token);
                int labelOPCode;
                if(strcmp(token, "\tpushbm") == 0){
                    labelOPCode = 6;
                }
                else if(strcmp(token, "\tpushsm") == 0){
                    labelOPCode = 7;
                }
                else if(strcmp(token, "\tpushim") == 0){
                    labelOPCode = 8;
                }
                else if(strcmp(token, "\tpushlm") == 0){
                    labelOPCode = 9;
                }
                else if(strcmp(token, "\tpushfm") == 0){
                    labelOPCode = 10;
                }
                else if(strcmp(token, "\tpushdm") == 0){
                    labelOPCode = 11;
                }
                else if(strcmp(token, "\tpushmm") == 0){
                    labelOPCode = 12;
                }
                else if(strcmp(token, "\tpopbm") == 0){
                    labelOPCode = 25;
                }
                else if(strcmp(token, "\tpopsm") == 0){
                    labelOPCode = 26;
                }
                else if(strcmp(token, "\tpopim") == 0){
                    labelOPCode = 27;
                }
                else if(strcmp(token, "\tpoplm") == 0){
                    labelOPCode = 28;
                }
                else if(strcmp(token, "\tpopfm") == 0){
                    labelOPCode = 29;
                }
                else if(strcmp(token, "\tpopdm") == 0){
                    labelOPCode = 30;
                }
                else if(strcmp(token, "\tpopmm") == 0){
                    labelOPCode = 31;
                }
                else if(strcmp(token, "\tjmp") == 0){
                    labelOPCode = 131;
                }
                else if(strcmp(token, "\tjz") == 0){
                    labelOPCode = 134;
                }
                else if(strcmp(token, "\tjnz") == 0){
                    labelOPCode = 135;
                }
                else if(strcmp(token, "\tjgt") == 0){
                    labelOPCode = 136;
                }
                else if(strcmp(token, "\tjlt") == 0){
                    labelOPCode = 137;
                }
                else if(strcmp(token, "\tjge") == 0){
                    labelOPCode = 138;
                }
                else if(strcmp(token, "\tjle") == 0){
                    labelOPCode = 139;
                }
                else if(strcmp(token, "\tcall") == 0){
                    labelOPCode = 140;
                }
                else{
                    myExit(1, writeFileName);
                }
                //returns label and/or label+arithmetic
                token = strtok(NULL, "$");
                //printf("str: %s", token);

                //printf("first tok: %s \n", token);
                //returns 0 or 1 if pass, -1 if fails
                int check = verifyLabelRegex(token);
                //printf("Check val: %d \n", check);

                int labelInd;
                if(check == -1){
                    myExit(3, outFileName);
                }
                //0 = no arithmetic just label
                bool found = false;
                if(check == 0){
                    for(int i = 0; i < countLabels; i++){
                        if(strcmp(token, allLabels[i].name)  == 0){
                            labelInd = i;
                            found = true;
                            int address = allLabels[i].address;
                            // printf("address: %d", address);
                            //write opcode to file
                            writeOPToFile(labelOPCode, f_out);
                            write24BitAddress(address, f_out);
                            continue;
                            //write address at label index to file (24 bit)
                        }
                    }
                    if(!found)
                        myExit(-1, outFileName);
                }
                //1 = arithmetic label addition
                if(check == 1){
                    token = strtok(token, "+");
                    // printf("second tok: %s \n", token);
                    size_t len2 = strlen(token);
                    char* labelNameCheck = (char*)malloc(len2 + 2);
                    strcpy(labelNameCheck, token);
                    labelNameCheck[len2] = '\n';
                    labelNameCheck[len2 + 1] = '\0';
                    token = strtok(NULL, "\n");
                    if(strlen(token) ==0){
                        myExit(-1, outFileName);
                    }

                    for(int i = 0; i<strlen(token); i++){
                        if(!isdigit(token[i]))
                            myExit(-1, outFileName);
                    }
                    //     else
                    //         myExit(-1, outFileName);
                    // }
                    bool found = false;
                    for(int i = 0; i < countLabels; i++){
                        if(strcmp(labelNameCheck, allLabels[i].name) == 0){
                            labelInd = i;
                            found = true;
                            // if(allLabels[i].type == 1){
                            //     if(atoi(token) != 1 || atoi(token) != 0 || atoi(token) != -1){
                            //         myExit(1, outFileName);
                            //     }
                            // }
                            
                            // if (allLabels[i].type != 0 && atoi(token) % allLabels[i].type == 0){
                            //     myExit(1, outFileName);
                            // }
                            
                            int address = allLabels[i].address + atoi(token);
                            // printf("address: %d", address);
                            writeOPToFile(labelOPCode,f_out);
                            write24BitAddress(address,f_out);
                            continue;
                            //write opcode to file
                            //write address at label index to file (24 bit)
                        }
                    }
                    if(!found)
                        myExit(-1, outFileName);
                }
                
                //2 = arithmetic label subtraction
                if(check == 2){
                    token = strtok(token, "-");
                    // printf("second tok: %s \n", token);
                    size_t len2 = strlen(token);
                    char* labelNameCheck = (char*)malloc(len2 + 2);
                    strcpy(labelNameCheck, token);
                    labelNameCheck[len2] = '\n';
                    labelNameCheck[len2 + 1] = '\0';
                    token = strtok(NULL, "\n");
                    //printf("HALLO tok %s \n", token);
                    if(strlen(token) == 0){
                        myExit(-1, outFileName);
                    }
                    for(int i = 0; i<strlen(token); i++){
                        if(!isdigit(token[i]))
                            myExit(-1, outFileName);
                    }
                    bool found = false;
                    for(int i = 0; i < countLabels; i++){
                        if(strcmp(labelNameCheck, allLabels[i].name) == 0){
                            
                            labelInd = i;
                            found = true;
                            // if(atoi(token) != allLabels[i].type){
                            //     myExit(1, outFileName);
                            // }

                            int address = allLabels[i].address - atoi(token);
                            // printf("address: %d", address);
                            writeOPToFile(labelOPCode,f_out);
                            write24BitAddress(address,f_out);
                            continue;
                            //write opcode to file
                            //write address at label index to file (24 bit)
                        }
                    }
                    if(!found)
                        myExit(-1, outFileName);
                }
            }
            int operand;
            long operandL;
            int items;
            int address;
            float valFloat;
            double value;
            long long tryMELongLong;

            int8_t instrIns = 0;
            switch(OPCODE){
                //opcodes w/ 8 bit value
                case 0:
                case 123:
                case 124:
                case 125:
                case 126:
                case 127:
                case 128:
                case 129:
                case 130:
                case 132: //this is uint technically, fixx if needed
                    get8BitOperand(buffer, &operand);
                    //write 8 bit int
                    if(operand >= INT8_MIN && operand <= INT8_MAX){
                        writeOPToFile(OPCODE, f_out);
                        write8BitOperand(operand, f_out);
                    }
                    else{
                        myExit(1, writeFileName);
                    }
                    break;
                
                //opcodes w/16 bit value
                case 1:
                  //  int operand;
                    get16BitOperand(buffer, &operand);
                    if(operand < INT16_MIN || operand > INT16_MAX){
                        myExit(1, writeFileName);
                    }
                    writeOPToFile(OPCODE, f_out);
                    write16BitOperand(operand, f_out);
                    break;

                //opcode w/32 bit value
                case 2:
                   // int operand;
                    get32BitOperand(buffer, &tryMELongLong);
                    if(tryMELongLong < INT32_MIN || tryMELongLong > INT32_MAX){
                        myExit(1, writeFileName);
                    }
                    int realOperand = (int) tryMELongLong;
                    writeOPToFile(OPCODE, f_out);
                    write32BitOperand(realOperand, f_out);
                    break;

                //opcode w/64 bit value
                case 3:
                   // long operand;
                    get64BitOperand(buffer, &tryMELongLong);
                    if(failed){
                        myExit(1,writeFileName);
                    }
                    if(tryMELongLong < INT64_MIN || tryMELongLong > INT64_MAX){
                        myExit(1, writeFileName);
                    }
                    long realOperandL = (long) tryMELongLong;
                    writeOPToFile(OPCODE, f_out);
                    write64BitOperand(realOperandL, f_out);
                    break;

                case 4:
                    get32BitValue(buffer, &valFloat);
                    // if(valFloat < FLT_MIN || valFloat > FLT_MAX){
                    //     remove(writeFileName);
                    //     //fprintf(stderr, "Error on line %d \n", (currLineNum-1));
                    //     return -1;
                    // }
                    writeOPToFile(OPCODE, f_out);
                    writeFloatToFile(valFloat, f_out);
                    break;

                case 5:
                    //printf("helloooo\n");
                    get64BitValue(buffer, &value);
                    // printf("value: %f \n", value);
                    // printf("value: %f \n", DBL_MIN);
                    // // if(value < DBL_MIN || operand > DBL_MAX){
                    //     remove(writeFileName);
                    //     //fprintf(stderr, "Error on line %d \n", (currLineNum-1));
                    //     return -1;
                    // }
                    writeOPToFile(OPCODE, f_out);
                    writeDoubleToFile(value, f_out);
                    break;

                //opcode w/address and items
                case 12:
                case 31:
                 //   int address;
                 //   int items;
                    
                    get24BitAddress(buffer, &address);
                    if(address < 0 || address > 16777215){
                        myExit(1, writeFileName);
                    }
                    
                    get8BitItem(temp, &items);
                    if(items < INT8_MIN || items > INT8_MAX){
                    myExit(1, writeFileName);
                    }
                    writeOPToFile(OPCODE, f_out);
                    write24BitAddress(address, f_out);
                    write8BitOperand(items, f_out);

                    break;

                //opcode w/24bit addresses
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 25:
                case 26:
                case 27:
                case 28:
                case 29:
                case 30:
                case 131:
                case 134:
                case 135:
                case 136:
                case 137:
                case 138:
                case 139:
                case 140:
                 //   int address;
                    getGeneral24BitAddress(buffer, &address);
                    if(address < 0 || address > 16777215){
                        myExit(1, writeFileName);
                    }
                    writeOPToFile(OPCODE, f_out);   
                    write24BitAddress(address, f_out);
                    break;

                //only instruction cases no operands/addresses cases 13-122
                case 13:
                case 14:
                case 15:
                case 16:
                case 17:
                case 18:
                case 19:
                case 20:
                case 21:
                case 22:
                case 23:
                case 24:
                case 32:
                case 33:
                case 34:
                case 35:
                case 36:
                case 37:
                case 38:
                case 39:
                case 40:
                case 41:
                case 42:
                case 43:
                case 44:
                case 45:
                case 46:
                case 47:
                case 48:
                case 49:
                case 50:
                case 51:
                case 52:
                case 53:
                case 54:
                case 55:
                case 56:
                case 57:
                case 58:
                case 59:
                case 60:
                case 61:
                case 62:
                case 63:
                case 64:
                case 65:
                case 66:
                case 67:
                case 68:
                case 69:
                case 70:
                case 71:
                case 72:
                case 73:
                case 74:
                case 75:
                case 76:
                case 77:
                case 78:
                case 79:
                case 80:
                case 81:
                case 82:
                case 83:
                case 84:
                case 85:
                case 86:
                case 87:
                case 88:
                case 89:
                case 90:
                case 91:
                case 92:
                case 93:
                case 94:
                case 95:
                case 96:
                case 97:
                case 98:
                case 99:
                case 100:
                case 101:
                case 102:
                case 103:
                case 104:
                case 105:
                case 106:
                case 107:
                case 108:
                case 109:
                case 110:
                case 111:
                case 112:
                case 113:
                case 114:
                case 115:
                case 116:
                case 117:
                case 118:
                case 119:
                case 120:
                case 121:
                case 122:
                case 133:
                case 141:
                case 142:
                    writeOPToFile(OPCODE, f_out);
                    break;
            }
        }
        else{
            continue;
        } //if not in a code segment then move on to the next line
    }
    
    rewind(file);
    resetGlobals();

    bool enterTheDataDir = false;
    
    while (fgets(buffer, sizeof(buffer), file) != NULL){
        currLineNum++;
        if(buffer[0] == ';' || buffer[0] == ':')
            continue;
        
        if(strcmp(buffer, ".byte\n") == 0){
            passedByteDirective = true;
            continue;
        }
        if(strcmp(buffer, ".ascii\n") == 0){
            passedASCIIDirective = true;
            continue;
        }
        if(strcmp(buffer, ".short\n") == 0){
            passedShortDirective = true;
            continue;
        }
        if(strcmp(buffer, ".int\n") == 0){
            passedIntDirective = true;
            continue;
        }
        if(strcmp(buffer, ".long\n") == 0){
            passedLongDirective = true;
            continue;
        }
        if(strcmp(buffer, ".float\n") == 0){
            passedFloatDirective = true;
            continue;
        }
        if(strcmp(buffer, ".double\n") == 0){
            passedDoubleDirective = true;
            continue;
        }

        if(strcmp(buffer, ".data\n") == 0){
            enterTheDataDir = true;
            continue;
        }
        if(strcmp(buffer, ".code\n") == 0){
            enterTheDataDir = false;
            continue;
        }
        if(enterTheDataDir){
            if(passedByteDirective){
                int dataItem = getByteNumber(buffer);

                int8_t byteNum;
                if (dataItem >= INT8_MIN && dataItem <= INT8_MAX) {
                    write8BitOperand(dataItem, f_out);
                }
                else{
                    myExit(1, writeFileName);
                }
                    // printf("ERORR FOR SIZE BYTE \n");
            }
            else if(passedASCIIDirective){
                if(strlen(buffer) != 3){
                    myExit(1, writeFileName);
                }
                int ascii = buffer[1];
                write8BitOperand(ascii, f_out);
            }
            else if(passedShortDirective){
                int dataItem = getShortNumber(buffer);
                

                if (dataItem >= INT16_MIN && dataItem <= INT16_MAX){
                    write16BitOperand(dataItem, f_out);
                }
                else{
                    myExit(1, writeFileName);
                }
                
                   // printf("ERORR FOR SIZE Short \n");
            }
            else if(passedIntDirective){
                int dataItem = getIntNumber(buffer);
                int32_t byteNum;
                if (dataItem >= INT32_MIN && dataItem <= INT32_MAX) {
                    write32BitOperand(dataItem, f_out);
                }
                else{
                    myExit(1, writeFileName);
                }
                
                    // printf("ERORR FOR SIZE Int \n");
            }
            else if(passedLongDirective){
                long dataItem = getLongNumber(buffer);
                int64_t byteNum;
                if (dataItem >= INT64_MIN && dataItem <= INT64_MAX) {
                    write64BitOperand(dataItem, f_out);
                }
                else{
                    myExit(1, writeFileName);
                }
                
                    // printf("ERORR FOR SIZE Long \n");
            }
            else if(passedFloatDirective){
                float dataItem = getFloatNumber(buffer);
                
                writeFloatToFile(dataItem, f_out);
                
                    // printf("ERORR FOR SIZE float \n");
            }
            else if(passedDoubleDirective){
                double dataItem = getDoubleNumber(buffer);
                writeDoubleToFile(dataItem, f_out);
                // if (dataItem >= INT64_MIN && dataItem <= INT64_MAX) {
                //     
                // }
                
                    // printf("ERORR FOR SIZE BYTE \n");
            }
           // else // put error here
                //printf("EVERYTHING FALSE! \n");
        }
    }
    return 0;
}



int verifyLabelRegex(char * str){
    char *regExpresses[3] = {
    "^:[a-zA-Z0-9_]+\n$",
    "^:[a-zA-Z0-9_]+[+][0-9]+\n$",
    "^:[a-zA-Z0-9_]+[-][0-9]+\n$"
    };

    for(int i = 0; i < 3; i++){
       // printf("check me %s \n" ,regExpresses[i]);
        if(verifyRegex(str, regExpresses[i]) == 0){
            return i;
        }
    }
    return -1;
}

void writeCodeDirHeader(FILE * file){
    uint32_t number = 8;
    uint8_t writeMe;
    for (int i = 3; i >= 0; i--) {
        writeMe = (number >> (i * 8)) & 0xFF;
        fwrite(&writeMe, sizeof(uint8_t), 1, file);
    }
}

void writeDataDirHeader(int counterCode, FILE * file){
    uint32_t number = (uint32_t) counterCode + 8;
    
    uint8_t writeMe;
    for (int i = 3; i >= 0; i--) {
        writeMe = (number >> (i * 8)) & 0xFF;
        fwrite(&writeMe, sizeof(uint8_t), 1, file);
    }
}

void writeFloatToFile(float number, FILE * file){
    
    unsigned char *ptr = (unsigned char *)&number;
    for (int i = sizeof(float) - 1; i >= 0; i--) {
        fwrite(&ptr[i], sizeof(unsigned char), 1, file);
    }
}

void writeDoubleToFile(double number, FILE * file){
    unsigned char *ptr = (unsigned char *)&number;
    for (int i = sizeof(double) - 1; i >= 0; i--) {
        fwrite(&ptr[i], sizeof(unsigned char), 1, file);
    }
}


void write32BitOperand(int operand, FILE * file){
    
    int32_t number = (int32_t) operand;
    //printf("Number: %d \n", number);
    
    uint8_t writeMe;
    for (int i = 3; i >= 0; i--) {
        writeMe = (number >> (i * 8)) & 0xFF;
        fwrite(&writeMe, sizeof(int8_t), 1, file);
    }
}

void write8BitOperand(int operand, FILE * file){
    int8_t number = (int8_t) operand;
    int8_t writeMe;
    for (int i = 0; i >= 0; i--){
        writeMe = (number >> (i * 8)) & 0xFF;
        fwrite(&writeMe, sizeof(int8_t), 1, file);
    }
}

void write16BitOperand(int operand, FILE * file){
    int16_t number = (int16_t) operand;
    int8_t writeMe;
    for (int i = 1; i >= 0; i--){
        writeMe = (number >> (i * 8)) & 0xFF;
        fwrite(&writeMe, sizeof(int8_t), 1, file);
    }
}

void write64BitOperand(long operandL, FILE * file){
    int64_t number = (int64_t) operandL;
    int8_t writeMe;
    for (int i = 7; i >= 0; i--){
        writeMe = (number >> (i * 8)) & 0xFF;
        fwrite(&writeMe, sizeof(int8_t), 1, file);
    }
}

void write24BitAddress(int address, FILE * file){
    // uint32_t number = (uint32_t) address;
    // uint8_t writeMe = (number) & 0xFF;
    // uint8_t writeMe2 = (number) & 0xFF00;
    // uint8_t writeMe3 = (number) & 0xFF0000;
    // fwrite(&writeMe3, sizeof(uint8_t), 1, file);
    // fwrite(&writeMe2, sizeof(uint8_t), 1, file);
    // fwrite(&writeMe, sizeof(uint8_t), 1, file);

    uint32_t masked_value = address & 0xFFFFFF;

    // Write each byte of the 24-bit value to the file
     for (int i = 2; i >= 0; i--) {
        uint8_t byte = (masked_value >> (i * 8)) & 0xFF;
        fwrite(&byte, sizeof(byte), 1, file);
    }
}


void writeOPToFile(int op, FILE * file){
    uint8_t OPcode = (uint8_t) op;
    uint8_t writeMe;
    for (int i = 0; i >= 0; i--){
        writeMe = (OPcode >> (i * 8)) & 0xFF;
        fwrite(&writeMe, sizeof(uint8_t), 1, file);
    }
}

int getByteNumber(char * str){
    // printf("len: %ld", strlen(str));
    if(strlen(str) == 2){
        myExit(1, outFileName);
    }
    char * token = strtok(str, "\t");
    token = strtok(token, "\n");

    for(int i = 0; i<strlen(token); i++){
        if(!isdigit(token[i]) && token[i] != '-'){
            myExit(1, outFileName);
        }
    }
    return atoi(token);
}
int getShortNumber(char * str){
    if(strlen(str) == 2){
        myExit(1, outFileName);
    }
    char * token = strtok(str, "\t");
    token = strtok(token, "\n");

    for(int i = 0; i<strlen(token); i++){
        if(!isdigit(token[i]) && token[i] != '-'){
            myExit(1, outFileName);
        }
    }
    return atoi(str);
}
int getIntNumber(char * str){
    if(strlen(str) == 2){
        myExit(1, outFileName);
    }
    char * token = strtok(str, "\t");
    token = strtok(token, "\n");
    int count = 0;
    for(int i = 0; i<strlen(token); i++){
        if(!isdigit(token[i]) && token[i] != '-'){
            myExit(1, outFileName);
        }
        count++;
    }
    if((token[0] == '-' && count > 11) || token[0] != '-'&& count > 10 ){
        myExit(1, outFileName);
    }
    return atoi(token);
}
long getLongNumber(char * str){
    if(strlen(str) == 2){
        myExit(1, outFileName);
    }
    char * token = strtok(str, "\t");
    token = strtok(token, "\n");
    int count = 0;
    for(int i = 0; i<strlen(token); i++){
        if(!isdigit(token[i]) && token[i] != '-'){
            myExit(1, outFileName);
        }
        count++;
    }
    if((token[0] == '-' && count > 20) || (token[0] != '-'&& count > 19)){
        myExit(1, outFileName);
    }
    return atol(token);
}
float getFloatNumber(char * str){
    if(strlen(str) == 2){
        myExit(1, outFileName);
    }
    char * token = strtok(str, "\t");
    token = strtok(token, "\n");
    for(int i = 0; i<strlen(token); i++){
        if(!isdigit(token[i]) && token[i] != '-'&& token[i] != '.' && token[i] != 'e' 
        && token[i] != '+'){
            myExit(1, outFileName);
        }
    }
    return strtof(token, NULL);
}
double getDoubleNumber(char * str){
    if(strlen(str) == 2){
        myExit(1, outFileName);
    }
    char * token = strtok(str, "\t");
    token = strtok(token, "\n");
    for(int i = 0; i<strlen(token); i++){
        if(!isdigit(token[i]) && token[i] != '-'&& token[i] != '.' && token[i] != 'e' 
        && token[i] != '+'){
            myExit(1, outFileName);
        }
    }
    return strtod(token, NULL);
}



void getGeneral24BitAddress(char *token, int * num){
    char * token2 = strtok(token, " ");
    token2 = strtok(NULL, "$");
    *num = atoi(token2);
}

void get24BitAddress(char *token, int *num){
    char * token2 = strtok(token, " ");
    token2 = strtok(NULL, " ");
    *num = atoi(token2);
}

void get8BitItem(char *token, int * num){
    char * token2 = strtok(token, " ");
    token2 = strtok(NULL, " ");
    token2 = strtok(NULL, "$");
    *num = atoi(token2);
}

void get64BitValue(char * token, double *val){
    char * token2 = strtok(token, " ");
    token2 = strtok(NULL, "$");
    *val = strtod(token2, NULL);
}
void get32BitValue(char * token, float* val){
    char * token2 = strtok(token, " ");
    token2 = strtok(NULL, "$");
    *val = strtof(token2, NULL);
}

void get64BitOperand(char *token, long long * num){
    char * token2 = strtok(token, " ");
    token2 = strtok(NULL, "$");
    if(strlen(token2) > 21){
        failed = true;
    }
    *num = strtoll(token2, NULL, 10);
}

void get32BitOperand(char *token, long long * num){
    char * token2 = strtok(token, " ");
    token2 = strtok(NULL, "$");
     *num = strtoll(token2, NULL, 10);
}


void get16BitOperand(char *token, int * num){
    char * token2 = strtok(token, " ");
    token2 = strtok(NULL, "$");
    *num = atoi(token2);
    
}

void get8BitOperand(char *token, int * num){
    char * token2 = strtok(token, " ");
    token2 = strtok(NULL, "$");
    *num = atoi(token2);
}
    

int opcodeFromInstruction(char * token){
    char *regExpresses[143] = {
    "^\tpushb (-?)[0-9]+\n",
    "^\tpushs (-?)[0-9]+\n",
    "^\tpushi (-?)[0-9]+\n", 
    "^\tpushl (-?)[0-9]+\n", 
    "^\tpushf [-+]?[0-9]+[.]?[0-9]*([eE][-+]?[0-9]+)?\n", 
    "^\tpushd [-+]?[0-9]+[.]?[0-9]*([eE][-+]?[0-9]+)?\n",
    "^\tpushbm (-?)[0-9]+\n",
    "^\tpushsm (-?)[0-9]+\n", 
    "^\tpushim (-?)[0-9]+\n", 
    "^\tpushlm (-?)[0-9]+\n", 
    "^\tpushfm (-?)[0-9]+\n",
    "^\tpushdm (-?)[0-9]+\n",
    "^\tpushmm (-?)[0-9]+ (-?)[0-9]+\n", 
    "^\tdupb\n", 
    "^\tdups\n", 
    "^\tdupi\n", 
    "^\tdupl\n", 
    "^\tdupf\n", 
    "^\tdupd\n", 
    "^\tpopb\n", 
    "^\tpops\n", 
    "^\tpopi\n",
    "^\tpopl\n",
    "^\tpopf\n", 
    "^\tpopd\n", 
    "^\tpopbm (-?)[0-9]+\n",
    "^\tpopsm (-?)[0-9]+\n", 
    "^\tpopim (-?)[0-9]+\n", 
    "^\tpoplm (-?)[0-9]+\n",
    "^\tpopfm (-?)[0-9]+\n",
    "^\tpopdm (-?)[0-9]+\n", 
    "^\tpopmm (-?)[0-9]+ (-?)[0-9]+\n", 
    "^\tswapb\n", 
    "^\tswaps\n", 
    "^\tswapi\n", 
    "^\tswapl\n", 
    "^\tswapf\n", 
    "^\tswapd\n",
    "^\tconvbs\n", 
    "^\tconvbi\n", 
    "^\tconvbl\n", 
    "^\tconvbf\n", 
    "^\tconvbd\n", 
    "^\tconvsb\n", 
    "^\tconvsi\n", 
    "^\tconvsl\n", 
    "^\tconvsf\n", 
    "^\tconvsd\n",
    "^\tconvib\n", 
    "^\tconvis\n", 
    "^\tconvil\n", 
    "^\tconvif\n", 
    "^\tconvid\n", 
    "^\tconvlb\n", 
    "^\tconvls\n", 
    "^\tconvli\n",
    "L manual",
    "^\tconvlf\n", 
    "^\tconvld\n",
    "\tconvfb", 
    "\tconvfs", 
    "\tconvfi", 
    "\tconvfl", 
    "\tconvfd", 
    "\tconvdb", 
    "\tconvds", 
    "\tconvdi", 
    "\tconvdl",
    "\tconvdf",
    "\tinch", 
    "\tinb", 
    "\tins", 
    "\tini", 
    "\tinl",
    "\tinf",
    "\tind",
    "\toutch",
    "\toutb",
    "\touts", 
    "\touti",
    "\toutl", 
    "\toutf", 
    "\toutd", 
    "\taddb", 
    "\tadds", 
    "\taddi", 
    "\taddl", 
    "\taddf", 
    "\taddd", 
    "\tsubb", 
    "\tsubs", 
    "\tsubi", 
    "\tsubl", 
    "\tsubf", 
    "\tsubd", 
    "\tmulb",
    "\tmuls", 
    "\tmuli", 
    "\tmull", 
    "\tmulf",
    "\tmuld", 
    "\tdivb", 
    "\tdivs", 
    "\tdivi", 
    "\tdivl", 
    "\tdivf",
    "\tdivd", 
    "\tand8", 
    "\tand16",
    "\tand32", 
    "\tand64",
    "\tor8", 
    "\tor16", 
    "\tor32", 
    "\tor64", 
    "\txor8", 
    "\txor16",
    "\txor32",
    "\txor64",
    "\tnot8", 
    "\tnot16", 
    "\tnot32",
    "\tnot64", 
    "\tshftrb (-?)[0-9]+", 
    "\tshftrs (-?)[0-9]+", 
    "\tshftri (-?)[0-9]+", 
    "\tshftrl (-?)[0-9]+", 
    "\tshftlb (-?)[0-9]+", 
    "\tshftls (-?)[0-9]+",
    "\tshftli (-?)[0-9]+", 
    "\tshftll (-?)[0-9]+", 
    "\tjmp (-?)[0-9]+", 
    "\tjrpc (-?)[0-9]+", 
    "\tjind", 
    "\tjz (-?)[0-9]+", 
    "\tjnz (-?)[0-9]+", 
    "\tjgt (-?)[0-9]+",
    "\tjlt (-?)[0-9]+", 
    "\tjge (-?)[0-9]+", 
    "\tjle (-?)[0-9]+",
    "\tcall (-?)[0-9]+",
    "\treturn", 
    "\thalt"
    };

    for(int i = 0; i < 143; i++){
       // printf("check me %s \n" ,regExpresses[i]);
        if(verifyRegex(token, regExpresses[i]) == 0){
            return i;
        }
    }
    return -1;
}

int myExit(int x, char* fName){
    if(x == 0){
        exit(0);
    }
    remove(fName);
    
    fprintf(stderr, "Error on line %d\n", currLineNum);
    exit(x);
}

int verifyRegex(char *token, char * regexPattern){

    regex_t testPattern;
    int reti2 = regcomp(&testPattern, regexPattern, REG_EXTENDED);
    int reti = regexec(&testPattern, token, 0, NULL, 0);
    if(reti == 0){
        regfree(&testPattern);
        return 0;
    }
    regfree(&testPattern);
    return -1;
}


void resetGlobals(){
    passedByteDirective, passedASCIIDirective, passedShortDirective, passedIntDirective, 
    passedLongDirective, passedFloatDirective, passedDoubleDirective = false;
    currLineNum = 0;
}
char * removeFirstChar(char *str) {
    int length = strlen(str);
    for (int i = 0; i < length; i++) {
        str[i] = str[i + 1];
    }
}
int findInstructionSize(char * str){
    if(strcmp(str, "\tpushb")  == 0 || strcmp(str, "\tjrpc")  == 0 || strcmp(str, "\tshftrb")  == 0 ||strcmp(str, "\tshftrs")  == 0 ||
    strcmp(str, "\tshftri")  == 0 ||strcmp(str, "\tshftrl")  == 0 ||strcmp(str, "\tshftlb")  == 0 ||strcmp(str, "\tshftls")  == 0 ||
    strcmp(str, "\tshftli")  == 0 ||strcmp(str, "\tshftll")  == 0){
        return 2;
    }
    else if(strcmp(str, "\tpushs")  == 0){
        return 3;
    }

    else if(strcmp(str, "\tpushbm")  == 0 || strcmp(str, "\tpushsm")  == 0 || strcmp(str, "\tpushim")  == 0 || strcmp(str, "\tpushlm")  == 0 || 
    strcmp(str, "\tpushfm")  == 0 || strcmp(str, "\tpushdm")  == 0 || strcmp(str, "\tjmp")  == 0 || strcmp(str, "\tjz")  == 0 || 
    strcmp(str, "\tjnz")  == 0 || strcmp(str, "\tjgt")  == 0 || strcmp(str, "\tjlt")  == 0 || strcmp(str, "\tjge")  == 0 || 
    strcmp(str, "\tjle")  == 0 || strcmp(str, "\tcall")  == 0 || strcmp(str, "\tpopbm")  == 0 || strcmp(str, "\tpopsm")  == 0 || 
    strcmp(str, "\tpopim")  == 0 || strcmp(str, "\tpoplm")  == 0 || strcmp(str, "\tpopfm")  == 0 || strcmp(str, "\tpopdm")  == 0){
        return 4;
    }
    else if(strcmp(str, "\tpushf")  == 0 || strcmp(str, "\tpushi")  == 0 || strcmp(str, "\tpushmm")  == 0 || strcmp(str, "\tpopmm")  == 0){
        return 5;
    }
    else if(strcmp(str, "\tpushd")  == 0 || strcmp(str, "\tpushl")  == 0){
        return 9;
    }
    else
        return 1;
}