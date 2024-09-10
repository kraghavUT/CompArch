#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <stdbool.h>
#include<ctype.h>
#include <stdlib.h>
#include <stdint.h>

  int countBytes(char *token);
  //int regExisValid(const char *patternsOfRegex[], char* checkMe, int numRegExes);
  int findNumberInString(char *str);
  void extractVals(char *str, int *r, int *s, long *L);
  void extractVals2(char *str, int *r, int *s, long *L);
  void longToBitString(long num, char *bitstring, int size);
  void substring(const char *source, int start, int length, char *dest);
  bool isRegexValid(const char *pattern,  char* checkMe);
  void parseInstr3Reg(char *str, int *rd, int *rs, int *rt);
  void parseInstr1Reg1Imm(char *str, int *rd, long *L);
  void parseInstr2Reg(char *str, int *rd, int *rs);
  void parseInstr1Reg(char *str, int *rd);
  void parseInstr1Imm(char *str, long*L);
  long findLongInString(char * str);
  int findIntInString(char * str);
  // int checkIfInRange(long L);

  int main(int argc, char** argv)
  {
    //Create an Array for Regex expressions, passed to a function
    //that will tell us the validity and opcode(arr ind)  

    const char *regExpresses[32] = {
      "^\tadd r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0])\n?$",
      "^\taddi r([0-9]|1[0-9]|2[0-9]|3[0]), [0-9]+\n?$",
      "^\tsub r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0])\n?$",
      "^\tsubi r([0-9]|1[0-9]|2[0-9]|3[0]), [0-9]+\n?$",
      "^\tmul r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0])\n?$",
      "^\tdiv r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0])\n?$",
      "^\tand r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0])\n?$",
      "^\tor r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0])\n?$",
      "^\txor r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0])\n?$",
      "^\tnot r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0])\n?$",
      "^\tshftr r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0])\n?$",
      "^\tshftri r([0-9]|1[0-9]|2[0-9]|3[0]), [0-9]+\n?$",
      "^\tshftl r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0])\n?$",
      "^\tshftli r([0-9]|1[0-9]|2[0-9]|3[0]), [0-9]+\n?$",
      "^\tbr r([0-9]|1[0-9]|2[0-9]|3[0])\n?$",
      "^\tbrr r([0-9]|1[0-9]|2[0-9]|3[0])\n?$",
      "^\tbrr -?[0-9]+\n?$",
      "^\tbrnz r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0])\n?$",
      "^\tcall r([0-9]|1[0-9]|2[0-9]|3[0])\n?$",
      "^\treturn\n?$",
      "^\tbrgt r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0])\n?$",
      "^\tmov r([0-9]|1[0-9]|2[0-9]|3[0]), \\(r[0-9]+\\)\\([0-9]+\\)\n?$",
      "^\tmov r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0])\n?$",
      "^\tmov r([0-9]|1[0-9]|2[0-9]|3[0]), [0-9]+\n?$",
      "^\tmov \\(r[0-9]+\\)\\([0-9]+\\), r([0-9]|1[0-9]|2[0-9]|3[0])\n?$",
      "^\taddf r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0])\n?$",
      "^\tsubf r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0])\n?$",
      "^\tmulf r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0])\n?$",
      "^\tdivf r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0])\n?$",
      "^\tin r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0])\n?$",
      "^\tout r([0-9]|1[0-9]|2[0-9]|3[0]), r([0-9]|1[0-9]|2[0-9]|3[0])\n?$",
      "^\thalt\n?$"
    };
    
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
  }label;

    int countLabels = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char check = ':';
        if(buffer[0] == check)
        {
          countLabels++;
        }
    }
    rewind(file);

    struct Label allLabels[countLabels];
    int labelIndex = 0;
    // TODO: 
    // if /t determine op code and comp with possible instructions: remmeber macro bytes
    // if number, then 64bit
    // pinpoint labels if they are in the file
    // place name in struct array of labels
    int byteCount = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if(buffer[0] == ':')
        {
          regex_t testReg;
          for (int i = 0; buffer[i] != '\0'; i++) {
              buffer[i] = buffer[i + 1];
          }
          int reti2 = regcomp(&testReg,"[a-zA-Z0-9_-]+", REG_EXTENDED);
          int reti = regexec(&testReg, buffer, 0, NULL, 0);
          if(reti != 0)
          {
            printf("failed label naming %s \n", buffer);
          }
          
          strcpy(allLabels[labelIndex].name, buffer);
          allLabels[labelIndex].address = byteCount;
          labelIndex++;
        }
        
        if(buffer[0] == '\t')
        {
          char *token = strtok(buffer, "\t");
          int numBytes = countBytes(token);
          byteCount += numBytes;
        }
    }
    rewind(file);

    //  printf("%s \n", allLabels[0].name);
    //  printf("%s \n", allLabels[1].name);
    //  printf("%d \n", allLabels[0].address);
    //  printf("%d \n", allLabels[1].address);
    //  printf("%d \n", byteCount);
    
    //write file
    FILE *f_out;
    
    char writeFileName[50];
    char temp [50];
    strcpy(writeFileName, argv[1]);
    int len = strlen(writeFileName);
    writeFileName[len] = 'o';
    writeFileName[len + 1] = '\0';

    // Open file in write mode
    f_out = fopen(writeFileName, "w");
    if (f_out == NULL) {
        perror("Failed to create file");
        return 1;
    }

    int currLineNum = 1;
    //set up the rest of the instructions includuing label stuff
    bool dataBool = false;
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
      currLineNum++;
      if(strcmp(buffer, ".data\n") ==0)
      {
        dataBool = true;
        continue;
      }
      if(buffer[0] == '\t' && dataBool)
      {
        regex_t regChec;
        int reti2 = regcomp(&regChec,"\t[1-9]+", REG_EXTENDED);
        int reti = regexec(&regChec, buffer, 0, NULL, 0);
        if(reti != 0)
        {
          remove(writeFileName);
          fprintf(stderr, "Error on line %d \n", (currLineNum-1));
          return -1;
        }
        char *token = malloc(sizeof(buffer));
        for(int i = 0; i< strlen(buffer); i++)
        {
          if(isdigit(buffer[i]))
          {
            token[0] = buffer[i];
          }
        }
        printf("HAI buffer: %s \n", buffer);
        uint64_t data = atol(buffer);
        
        fwrite((const void *) &data, sizeof(data), 1, f_out);

        continue;
       
      }
      if(buffer[0] == ';' || buffer[0] == ':')
      {
        continue;
      }
      if(strcmp(buffer, ".code\n") ==0)
      {
        dataBool = false;
        continue;
      }
      
      int indexOfRegex = -1;
      for(int i = 0; i < 32; i++)
      {
        if(isRegexValid(regExpresses[i], buffer))
        {
          //printf("comparison: %s \n", regExpresses[i]);
          indexOfRegex = i;
          break;
        }
      }
      printf("Buffer: %s \n opcode: %d \n", buffer, indexOfRegex);
      

      //int indexOfRegex = regExisValid(regExpresses, buffer, 32); //THIS IS THE OPCODE NUMBER 
      //printf("Opcode: %d \n", indexOfRegex);
      // printf("%s \n", buffer);

  //    int reti = regexec(&regexes[0], buffer, 0 , NULL, 0);
      //printf("%d reti", reti);
      if(indexOfRegex > -1)
      {
        if(indexOfRegex == 21)
        {
          //printf("enterd the 21");
            //mov rd, (rs)(L)
            int rD, rS;
            long L;
            extractVals(buffer, &rD, &rS, &L);
            uint64_t sign = (L & (1 << 11));
            sign = sign >> 11;
            if(sign ==1 )
            {
              L = ~(L+4097);
            }
            printf("opcode: %d \n", indexOfRegex);
            printf("rd: %d \n", rD);
            printf("rs: %d \n", rS);
            printf("L: %ld \n", L);

            uint32_t instr = 0;
            instr |= (indexOfRegex & 0x1f) << 27;
            instr |= (rD & 0x1f) << 22;
            instr |= (rS & 0x1f) << 17;
            instr |= (0 & 0x1f) << 12;
            instr |= L & 0xFFF;
            fwrite((const void *) &instr, sizeof(instr), 1, f_out);
            continue;
            // printf("This ind works: %d \n", indexOfRegex);
        }

        if(indexOfRegex == 24)
        {
            printf("entered the 24");
            //mov (rd)(L), rs)
            int rD, rS;
            long L;
            
            extractVals2(buffer, &rD, &rS, &L);
            // int check = checkIfInRange(L);
            // if(check == 1)
            // {
            //   //error message
            // }
            uint64_t sign = (L & (1 << 11));
            sign = sign >> 11;
            if(sign ==1 )
            {
              L = ~(L+4097);
            }
            uint32_t instr = 0;
            printf("opcode: %d \n", indexOfRegex);
            printf("rd: %d \n", rD);
            printf("rs: %d \n", rS);
            printf("L: %ld \n", L);
            instr |= (indexOfRegex & 0x1f) << 27;
            instr |= (rD & 0x1f) << 22;
            instr |= (rS & 0x1f) << 17;
            instr |= (0 & 0x1f) << 12;
            instr |= L & 0xFFF;
            fwrite((const void *) &instr, sizeof(instr), 1, f_out);
            continue;
            // printf("This ind works: %d \n", indexOfRegex);
        }

        int rd, rs, rt;
        long L;
        char *token = strtok(buffer, "\t");        
        // parseInstr(token, &rd, &rs, &rt, &L);

        
        uint32_t instr = 0;
        switch(indexOfRegex)
        {
          case 0:
          case 2:
          case 4:
          case 5:
          case 6:
          case 7:
          case 8:
          case 10:
          case 12:
          case 20:
          case 26:
          case 27:
          case 28:
          case 29:
          case 25:
            //three registers goes into array rD, rS, R
            parseInstr3Reg(token, &rd, &rs, &rt);
            printf("opcode: %d \n", indexOfRegex);
            printf("rd: %d \n", rd);
            printf("rs: %d \n", rs);
            printf("rt: %d \n", rt);
            instr |= (indexOfRegex & 0x1f) << 27;
            instr |= (rd & 0x1f) << 22;
            instr |= (rs & 0x1f) << 17;
            instr |= (rt & 0x1f) << 12;
            instr |= 0 & 0xFFF;

            //printf("%d INSTRUCTION \n", instr);
            //write to file
            fwrite((const void *) &instr, sizeof(instr), 1, f_out);
            // printf("This ind works: %d \n", indexOfRegex);
            break;
          case 1:
          case 3:
          case 11:
          case 13:
          case 23:
            //1 register, 1 immediate
            parseInstr1Reg1Imm(token, &rd, &L);
            printf("opcode: %d \n", indexOfRegex);
            printf("rd: %d \n", rd);
            printf("L: %ld \n", L);
            instr |= (indexOfRegex & 0x1f) << 27;
            instr |= (rd & 0x1f) << 22;
            instr |= L & 0xFFF;
            //write to file
            fwrite((const void *) &instr, sizeof(instr), 1, f_out);
            // printf("This ind works: %d \n", indexOfRegex);
            break;
          case 9:
          case 17:
          case 30:
          case 22:
            //2 registers
            parseInstr2Reg(token, &rd, &rs);
            printf("opcode: %d \n", indexOfRegex);
            printf("rd: %d \n", rd);
            printf("rs: %d \n", rs);
            instr |= (indexOfRegex & 0x1f) << 27;
            instr |= (rd & 0x1f) << 22;
            instr |= (rs & 0x1f) << 17;
            instr |= (0 & 0x1f) << 12;
            instr |= 0 & 0xFFF;
            //write to file
            fwrite((const void *) &instr, sizeof(instr), 1, f_out);
            // printf("This ind works: %d \n", indexOfRegex);
            continue;
          
          case 14:
          case 15:
          case 18:
            //1 register
            parseInstr1Reg(token, &rd);
            printf("opcode: %d \n", indexOfRegex);
            printf("rd: %d \n", rd);
            instr |= (indexOfRegex & 0x1f) << 27;
            instr |= (rd & 0x1f) << 22;
            instr |= (0 & 0x1f) << 17;
            instr |= (0 & 0x1f) << 12;
            instr |= 0 & 0xFFF;
            //write to file
            fwrite((const void *) &instr, sizeof(instr), 1, f_out);
            // printf("This ind works: %d \n", indexOfRegex);
            continue;

          case 16:
            //1 immediate
            parseInstr1Imm(token, &L);
            printf("opcode: %d \n", indexOfRegex);
            printf("L: %ld \n", L);
            instr |= (indexOfRegex & 0x1f) << 27;
            instr |= (0 & 0x1f) << 22;
            instr |= (0 & 0x1f) << 17;
            instr |= (0 & 0x1f) << 12;
            instr |= L & 0xFFF;
            //write to file
            fwrite((const void *) &instr, sizeof(instr), 1, f_out);
            // printf("This ind works: %d \n", indexOfRegex);
            continue;

          case 19:
          case 31: 
            //no register or immediate
            instr |= (indexOfRegex & 0x1f) << 27;
            instr |= (0 & 0x1f) << 22;
            instr |= (0 & 0x1f) << 17;
            instr |= (0 & 0x1f) << 12;
            instr |= 0 & 0xFFF;
            //write to file
            fwrite((const void *) &instr, sizeof(instr), 1, f_out);
            // printf("This ind works: %d \n", indexOfRegex);
            continue;
        }
      }

      
      
      //MACRO OR invalid regex and fails
      else
      {

        const char *macroExp[5] = {
          "\tclr r([0-9]|1[0-9]|2[0-9]|3[0])\n$",
          "\tld r([0-9]|1[0-9]|2[0-9]|3[0]), [0-9]+\n$",
          "\tld r([0-9]|1[0-9]|2[0-9]|3[0]), :[a-zA-Z0-9_-]+\n$",
          "\tpush r([0-9]|1[0-9]|2[0-9]|3[0])\n$",
          "\tpop r([0-9]|1[0-9]|2[0-9]|3[0])\n$"
        };

        //MACRO
       
        int indexOfRegex = -1;
        for(int i = 0; i < 5; i++)
        {
          if(isRegexValid(macroExp[i], buffer))
          {
            indexOfRegex = i;
            break;
          }
        }
        printf("pre tok: %s\n", buffer);

        if(indexOfRegex == -1)
        {
          remove(writeFileName);
          fprintf(stderr, "Error on line %d \n", (currLineNum-1));
          return -1;
        }
        
        char *token = strtok(buffer, " ");
        printf("Token: %s\n", token);
        if(strcmp(token, "\tclr") == 0)
        {
          
          token = strtok(NULL, "$");
          printf("Token: %s\n", token);
          
          int regD = findIntInString(token);
          int opcode = 8;
          uint32_t instr = 0;
          printf("opcode: %d \n", opcode);
          printf("rd: %d \n", regD);
          instr |= (opcode & 0x1f) << 27;
          instr |= (regD & 0x1f) << 22;
          instr |= (regD & 0x1f) << 17;
          instr |= (regD & 0x1f) << 12;
          instr |= 0 & 0xFFF;
          //printf("%d INSTRUCTION \n", instr);
          //write to file
          fwrite((const void *) &instr, sizeof(instr), 1, f_out);
         // printf("CLR complete \n");
        }

        else if(strcmp(token, "\tld") == 0)
        {
          token = strtok(NULL, ",");
          int regD = findNumberInString(token);
          token = strtok(NULL, "$");
         // printf("Pretok: %s \n", token);
          for (int i = 0; token[i] != '\0'; i++) {
              token[i] = token[i + 1];
          }
          for (int i = 0; token[i] != '\0'; i++) {
              token[i] = token[i + 1];
          }
          printf("Token: %s \n", token);
          
          int findLabelIndex = -1;
          for(int i = 0; i < sizeof(allLabels); i++)
          {
            if(strcmp(allLabels[i].name, token) == 0)
            {
              findLabelIndex = i;
              break;
            }
          }
          printf("Label index: %d \n", findLabelIndex);
          regex_t regCheck;
          char *checker;
          long immediateVal;
          if(findLabelIndex > -1)
          {
            immediateVal = allLabels[findLabelIndex].address;
          }

          else
          {
            int reti2 = regcomp(&regCheck,"0|[1-9]\\d{0,2}|[1-3]\\d{3}|409[0-6]", REG_EXTENDED);
            int reti = regexec(&regCheck, token, 0, NULL, 0);
          //  printf("LD MACRO regex: %d \n", reti);
          
            if(reti != 0)
            {
             // printf("invalid immediate value at LD macro");
            }
            immediateVal = strtol(token, &checker, 10);
          }

          printf("Imm val %ld \n" ,immediateVal);
          
          int size = sizeof(immediateVal) * 8;
          char bitstring[size + 1];
          longToBitString(immediateVal, bitstring, size);
          //printf("Bit string %s \n" , bitstring);
          
          int opcodeCLR = 8;
          int opcodeANDI = 1;
          int opcodeShiftli = 13;
          int ldInstrs[12]; 
          
          int start = 0; // Start index of the substring
          int length = 12; // Length of the substring
          char dest[length + 1]; // +1 for null terminator

          for(int i = 0; i<12; i++)
          {
            //printf("Bit string %s \n" , bitstring);
            if(i == 0) //clr
            {
              uint32_t instr = 0;
              instr |= (opcodeCLR & 0x1f) << 27;
              instr |= (regD & 0x1f) << 22;
              instr |= (regD & 0x1f) << 17;
              instr |= (regD & 0x1f) << 12;
              instr |= 0 & 0xFFF;
              ldInstrs[i] = instr;
              continue;
            }
            if(i % 2 == 1)//addi
            {
              if(i == 11)
              {
                length = 4;
                substring(bitstring, start, length, dest);
                //printf("Dest final: %s \n", dest);
                uint32_t instr = 0;
                instr |= (opcodeANDI & 0x1f) << 27;
                instr |= (regD & 0x1f) << 22;
                instr |= atol(dest) & 0xFFF;
                ldInstrs[i] = instr;
                continue;
              }
              substring(bitstring, start, length, dest);
              //printf("Dest: %s \n", dest);

              uint32_t instr = 0;
              instr |= (opcodeANDI & 0x1f) << 27;
              instr |= (regD & 0x1f) << 22;
              instr |= atol(dest) & 0xFFF;
              ldInstrs[i] = instr;
            }
            else //shftli
            {
              uint32_t instr = 0;
              instr |= (opcodeShiftli & 0x1f) << 27;
              instr |= (regD & 0x1f) << 22;
              instr |= atol(dest) & 0xFFF;
              ldInstrs[i] = instr;
              start += 12;
            }
            
          }
          for(int i = 0; i < sizeof(ldInstrs); i++)
          {
            fwrite((const void *) &ldInstrs[i], sizeof(ldInstrs[i]), 1, f_out);
          }
        }
        else if(strcmp(token, "push") == 0)
        {
          int pushInstrs[2]; 
          int opmov = 24;
          int opsubi = 3;
          token = strtok(NULL, "$");
          printf("Token hai: %s\n", token);
          while (*token != '\0') {
            *token = *(token + 1);
            token++;
          }
          int regD = atoi(token);
          long immediate = -8;
          int firstReg = 31;
          uint32_t instr = 0;
          instr |= (opmov & 0x1f) << 27;
          instr |= (firstReg & 0x1f) << 22;
          instr |= (regD & 0x1f) << 17;
          instr |= (0 & 0x1f) << 12;
          instr |= immediate & 0xFFF;
          pushInstrs[0] = instr;

          uint32_t instr2 = 0;
          instr2 |= (opsubi & 0x1f) << 27;
          instr2 |= (firstReg & 0x1f) << 22;
          instr2 |= (0 & 0x1f) << 17;
          instr2 |= (0 & 0x1f) << 12;
          instr2 |= immediate & 0xFFF;
          pushInstrs[1] = instr2;

          continue;

        }
        else if(strcmp(token, "pop") == 0)
        {
          int popInstrs[2]; 
          int opmov = 22;
          int opaddi = 1;
          token = strtok(NULL, "$");
          printf("Token: %s\n", token);
          while (*token != '\0') {
            *token = *(token + 1);
            token++;
          }
          int regD = atoi(token);
          long immediate = -8;
          int firstReg = 31;
          uint32_t instr = 0;
          instr |= (opmov & 0x1f) << 27;
          instr |= (regD & 0x1f) << 22;
          instr |= (firstReg & 0x1f) << 17;
          instr |= (0 & 0x1f) << 12;
          instr |= immediate & 0xFFF;
          popInstrs[0] = instr;

          uint32_t instr2 = 0;
          instr2 |= (opaddi & 0x1f) << 27;
          instr2 |= (firstReg & 0x1f) << 22;
          instr2 |= (0 & 0x1f) << 17;
          instr2 |= (0 & 0x1f) << 12;
          instr2 |= immediate & 0xFFF;
          popInstrs[1] = instr2;
          continue;

        }


        else{
          remove(writeFileName);
          fprintf(stderr, "Error on line %d \n", (currLineNum-1));
          return -1;
        }
      }
    }

    return 0;
  }

  void parseInstr3Reg(char *str, int *rd, int *rs, int *rt)
  {
    char* token = strtok(str, " ");
    token = strtok(NULL, ",");
    printf("latest token: %s \n", token);
    *rd = findIntInString(token);
    
    token = strtok(NULL, ",");

    printf("latest token: %s \n", token);
    
    *rs = findNumberInString(token);
    token = strtok(NULL, ",");
    printf("latest token: %s \n", token);
    *rt = findNumberInString(token);
  }

  void parseInstr1Reg1Imm(char *str, int *rd, long *L)
  {
    char* token = strtok(str, " ");
    token = strtok(NULL, ",");
    printf("latest token: %s \n", token);
    *rd = findIntInString(token);

    token = strtok(NULL, "$");
    printf("latest token: %s \n", token);
    *L = findLongInString(token);
  }

  void parseInstr2Reg(char *str, int *rd, int *rs)
  {
    char* token = strtok(str, " ");
    token = strtok(NULL, ",");
    // printf("latest token: %s \n", token);
    *rd = findIntInString(token);
    
    token = strtok(NULL, ",");

   // printf("latest token: %s \n", token);
    
    *rs = findNumberInString(token);
  }
  void parseInstr1Reg(char *str, int *rd)
  {
    char* token = strtok(str, " ");
    token = strtok(NULL, "$");
    *rd = findIntInString(token);
  }

  void parseInstr1Imm(char *str, long*L)
  {
    char* token = strtok(str, " ");
    token = strtok(NULL, "$");
    *L = atol(token);
  }


  int findIntInString(char * str)
  {
    for(int i = 0; i<strlen(str); i++)
    {
      str[i] = str[i+1];
    }
    return atoi(str);
  }


  long findLongInString(char * str)
  {
    for(int i = 0; i<strlen(str); i++)
    {
      str[i] = str[i+1];
    }
    return atol(str);
  }

  void substring(const char *source, int start, int length, char *dest){
    strncpy(dest, source + start, length);
    dest[length] = '\0'; 
  }
  void longToBitString(long num, char *bitstring, int size) {
    for (int i = size - 1; i >= 0; i--) {
        bitstring[i] = (num & 1) ? '1' : '0';
        num >>= 1;
    }
    bitstring[size] = '\0';
  }
  void extractVals2(char *str, int *r, int *s, long *L)
  {
    //mov (rd)(L), rs
   // printf("here and now: %s \n", str);
    char * temp = strtok(str, "\t");
   // printf("here and now: %s \n", temp);
    temp = strtok(temp, " ");
   // printf("here and now: %s \n", temp);
    temp = strtok(NULL, ")");
   // printf("here and now: %s \n", temp);
    *r = findNumberInString(temp);
    temp = strtok(NULL, ")");
    //printf("here and now: %s \n", temp);
    *L = findLongInString(temp);
    temp = strtok(NULL, " ");
    //printf("here and now: %s \n", temp);
    temp = strtok(NULL, "$");
    //printf("here and now: %s \n", temp);
    *s = findIntInString(temp);
  }

  void extractVals(char *str, int *r, int *s, long *L)
  {
    //printf("right now: %s \n", str);
    char * temp = strtok(str, "\t");
    temp = strtok(temp, " ");
    //printf("right now: %s \n", temp);
    temp = strtok(NULL, ",");
   // printf("right now: %s \n", temp);
    *r = findIntInString(temp);

    temp = strtok(NULL, "(");
    temp = strtok(NULL, ")");
   // printf("right now: %s \n", temp);
    *s = findIntInString(temp);
    
    temp = strtok(NULL, ")");
   // printf("right now: %s \n", temp);

    // temp = strtok(NULL, ")");
    *L = findLongInString(temp);

    // temp = strtok(NULL, ")");
    // findLongInString(temp);
    // *L = findLongInString(temp);
  }

int findNumberInString(char *str) {
    if (str == NULL || *str == '\0') {
        return -1; // Empty string or NULL pointer
    }
    // Move two chars to the right
    memmove(str, str + 2, strlen(str) - 1);
    return atoi(str);
}

  bool isRegexValid(const char *pattern, char* checkMe)
  {
    regex_t testPattern;
   // printf("Check Me: %s \n", checkMe);
    //printf("Pattern: %s \n", pattern);
    int reti2 = regcomp(&testPattern, pattern, REG_EXTENDED);
    int reti = regexec(&testPattern, checkMe, 0, NULL, 0);
    if(reti == 0)
    {
      regfree(&testPattern);
      return true;

    }
    regfree(&testPattern);
    return false;

  }

  //returns the index of the match if match
  // int regExisValid(const char *patterns[], char* checkMe, int numRegExes)
  // {
  //   regex_t testPattern;
  //   for (int i = 0; i < numRegExes; i++) {
  //     int reti2 = regcomp(&testPattern, patterns[i], REG_EXTENDED);
  //     int reti = regexec(&testPattern, checkMe, 0, NULL, 0);
      
  //     if (reti == 0) {
  //       regfree(&testPattern);
  //       return i; // String matches one of the regular expressions
  //     }
  //     regfree(&testPattern);
  //   }
  //   return -1; // String does not match any of the regular expressions
  // }

  int countBytes(char *token)
  {

    // printf("Token in count: %s \n", token);
    // printf("First char: %c \n", token[0]);

    bool isNum = false;
    for(int i = 0; token[i] != '\0'; i++)
    {
      if(isdigit(token[i]))
      {
        isNum = true;
      }
      else
        break;
    }
    if(isNum)
    {  
      return 8;
    }

    if(token[0] == 'c')
      return 4;
    else if(token[0] == 'l')
      return 48;
    else if(token[0] == 'p')
      return 8;
    else
      return 4;
  }

  