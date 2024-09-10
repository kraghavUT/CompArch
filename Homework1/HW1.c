#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h> 
#include <ctype.h>

int main()
{
  char OGmessage[] = "this is an array";  /* define an array*/
  char *pointMessage = "this is a pointer to that array"; /* define a pointer*/
  char **pointToPM = "pointer to poitn message";
  char ***pointToPM_PM = "pointer to the pointer that pointsto the pointer of the array";
  char ****pointToPM_PM_PM = "ultimate pointer to pointer to pointer to blahhhhhh";
  
  
  return 0;


}


// typedef struct Date{
//     int day;
//     int month;
//     int year;
//     float stockPrice;
// }fileDate;

// struct testDate{
//     int day;
//     int month;
//     int year;
    
// }inDate;

// int countlines(char *filename);
// bool checkInRange(struct testDate);
// int binarySearch(const fileDate *dates, struct testDate, int size);
// int compareDates(struct Date, struct testDate);

// int main()
// {

//   //setting up file and parsing file
//     FILE *filePointer;
//     filePointer = fopen("SPX.csv", "r");
    
//     if(filePointer == NULL)
//     {
//         printf("File not found \n");
//     }
//     int numLines = countlines("SPX.csv");
    
//     char inData[100];
//     fscanf(filePointer, "%s", inData); //first line is the Date,Close formatting || dont need
  
//     struct Date d;
//     int i;
//     fileDate* parsedFile = malloc((numLines) * sizeof *parsedFile);
//     for(i = 0; i < numLines; i++)
//     {
//       fscanf(filePointer, "%s", inData);
//       char *token;
//       token = strtok(inData, "-");
//       d.year = atoi(token);

//       token = strtok(NULL, "-");
//       d.month = atoi(token);

//       token = strtok(NULL, ",");
//       d.day = atoi(token);

//       token = strtok(NULL, "\n");
//       d.stockPrice = atof(token);

//       parsedFile[i] = d;
//     }

//     //User Input Managing
//     char userInput[100];
//     while(true)
//     {

//       printf("Enter a date in the format MM/DD/YYYY:\n");
//       scanf("%s", userInput);
//       if(strcmp(userInput, "done") == 0)
//       {
//         break;
//       }

//       if(strlen(userInput) > 10)
//       {
//         printf("unrecognized date format\n");
//         continue;
//       }
        
//       struct testDate inD;
//       bool checkValid;
//       checkValid = false;
//       char *token;
//       token = strtok(userInput, "/");
//       if(strlen(token) != 2)
//         checkValid = true;
//       for (int i = 0; i < strlen(token); i++) {
//         if(!isdigit(token[i]))
//         {
//           checkValid = true;
//         } 
//       }
//       if(atoi(token) < 1 || atoi(token) > 12)
//       {
//         checkValid = true;
//       }
      
//       if(checkValid)
//       {
//         printf("unrecognized date format\n");
//         continue;
//       }
//       inD.month = atoi(token);

//       token = strtok(NULL, "/");
//       if(strlen(token) != 2)
//         checkValid = true;
//       for (int i = 0; i < strlen(token); i++) {
//         if(!isdigit(token[i]))
//         {
//           checkValid = true;
//         }
//       }
//       if(atoi(token) < 1 || atoi(token) > 31)
//       {
//         checkValid  = true;
//       }
      
//       if(checkValid)
//       {
//         printf("unrecognized date format\n");
//         continue;
//       }
//       inD.day = atoi(token);
      
//       token = strtok(NULL, "/");
//       if(strlen(token) != 4)
//         checkValid = true;
//       for (int i = 0; i < strlen(token); i++) {
//         if(!isdigit(token[i]))
//         {
//           checkValid = true;
//         }
//       }
//       if(checkValid)
//       {
//         printf("unrecognized date format\n");
//         continue;
//       }
//       inD.year = atoi(token);

//       if(!checkInRange(inD))
//       {
//         printf("date out of range\n");
//         continue;
//       }
//  //     printf("%d %d %d", inD.month, inD.day, inD.year);

//       int index;
//       index = binarySearch(parsedFile, inD, numLines);
//  //     printf("%d index \n", index);

//       if(parsedFile[index].day > inD.day)
//       {
//         if(!(inD.month > parsedFile[index].month) && !(inD.year > parsedFile[index].month))
//         {
//           index--;
//         }
//       }

//       float lastClose = parsedFile[index].stockPrice;
//       char price[100];
//       sprintf(price, "%f", lastClose);

//       printf("%s\n", price);
//     //  printf("%d %d %d \n", inD.month, inD.day, inD.year);
//     }
    
//     free(parsedFile);
//     fclose(filePointer);
//     return 0;
// }


// int binarySearch(const fileDate *dates, struct testDate d1, int size)
// {
//     int low = 0;
//     int high = size - 1;
//     int tempMid;

//     while (low <= high) {
//         int mid = (low + high) / 2;
//         struct Date d;
//         d = dates[mid];
//     //  printf("%d %d %d ", d.month, d.day, d.year);
        
//         int comparison = compareDates(d, d1);
//         if (comparison == 0) {
//             return mid; 
//         } else if (comparison < 0) {
//             low = mid + 1;
//         } else {
//             high = mid - 1;
//         }
//         tempMid = mid;
//     }
//     return tempMid;
// }

// // -1 if d1 < d2
// // 0 if d1 == d2
// // 1 if d1 > d2
// int compareDates(struct Date d1, struct testDate d2)
// {
//     if(d1.year != d2.year)
//     {
//       if(d1.year < d2.year)
//       {
//         return -1;
//       }
//       else
//         return 1;
//     }

//     if(d1.month != d2.month)
//     {
//       if(d1.month < d2.month)
//       {
//         return -1;
//       }
//       else
//         return 1;
//     }

//     if(d1.day != d2.day)
//     {
//       if(d1.day < d2.day)
//       {
//         return -1;
//       }
//       else
//         return 1;
//     }
//     return 0;
// }

// bool checkInRange(struct testDate d1)
// {
//   int day;
//   int month;
//   int year;
//   day = d1.day;
//   month = d1.month;
//   year = d1.year;

//   if(day < 1 || day > 31)
//     return false;

//   if(month < 1 || month > 12)
//     return false;

//   if(year < 2020 || year > 2023)
//     return false;

//   return true;
// }

// int countlines(char *filename)
// {
//   FILE *fp = fopen(filename,"r");

//   char ch;
//   int count = 0;

//   while ((ch = fgetc(fp)) != EOF) {
//     if (ch == '\n') {
//         count++;
//     }
//   }
//   fclose(fp);
//   return count;
// }

