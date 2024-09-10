
#include "libtdmm/tdmm.h"
#include <stdio.h>
#include <time.h>

clock_t  startTime, endTime;

int main()
{
  void* stack_bot;
  t_init(FIRST_FIT, stack_bot);

  // t_malloc(1000);
  // void * m5  = t_malloc(200);
  // void * m7  = t_malloc(16);  
  // t_free(m5);
  // t_free(m7);
  // void * m2  = t_malloc(500);
  // void * m6  = t_malloc(300);
  // void * m9  = t_malloc(4);
  // t_free(m9);
  // void * m10  = t_malloc(500);
  // void * m8  = t_malloc(300);
  // t_free(m10);
  // void * m4  = t_malloc(1000);
  // void * m11  = t_malloc(200);
  // t_free(m2);
  // t_free(m3);
  // t_free(m6);
  // void * m1  = t_malloc(1000);
  // t_free(m8);
  // t_free(m1);
  // t_free(m4);
  // t_free(m11);
  

  // void * m3_1  = t_malloc(4000);
  // void * m5_1  = t_malloc(63);
  // void * m1_1  = t_malloc(1000);
  // void * m4_1  = t_malloc(125);
  // void * m11_1  = t_malloc(1);
  // void * m2_1  = t_malloc(500);
  // void * m6_1  = t_malloc(32);
  // void * m9_1  = t_malloc(4);
  // void * m7_1  = t_malloc(16);  
  // void * m10_1  = t_malloc(2);
  // void * m8_1 = t_malloc(8);

  // void * m3_2  = t_malloc(250);
  // void * m5_2  = t_malloc(63);
  // void * m1_2  = t_malloc(1000);
  // void * m4_2  = t_malloc(125);
  // void * m11_2  = t_malloc(1);
  // void * m2_2 = t_malloc(500);
  // void * m6_2  = t_malloc(32);
  // void * m9_2  = t_malloc(4);
  // void * m7_2  = t_malloc(16);  
  // void * m10_2  = t_malloc(2);
  // void * m8_2 = t_malloc(8);
 
  

  // t_free(m5_1);
  // t_free(m6_1);
  // t_free(m7_1);
  // t_free(m9_1);
  // t_free(m10_1);
  // t_free(m2_1);
  // t_free(m3_1);
  // t_free(m8_1);
  // t_free(m1_1);
  // t_free(m4_1);
  // t_free(m11_1);

  // t_free(m5_2);
  // t_free(m6_2);
  // t_free(m7_2);
  // t_free(m9_2);
  // t_free(m10_2);
  // t_free(m2_2);
  // t_free(m3_2);
  // t_free(m8_2);
  // t_free(m1_2);
  // t_free(m4_2);
  // t_free(m11_2);

  // void * final  = t_malloc(50);
  // void * m2  = t_malloc(60);
  // void * m3  = t_malloc(80);
  // void * m4  = t_malloc(50);
  // void * m5  = t_malloc(60);
  // void * m6  = t_malloc(80);
  // void * m7  = t_malloc(50);
  // void * m8  = t_malloc(60);
  // void * m9  = t_malloc(80);
  // void * m10  = t_malloc(50);
  // void * m11  = t_malloc(60);
  // void * m12  = t_malloc(80);
  // void * m13  = t_malloc(50);
  // void * m14 = t_malloc(60);
  // void * m15  = t_malloc(80);
  // void * m16 = t_malloc(50);
  // void * m17  = t_malloc(60);
  // void * m18  = t_malloc(80);
  // void * m19  = t_malloc(50);
  // void * m20  = t_malloc(60);
  // void * m21  = t_malloc(80);

  // t_free(m12);
  // t_free(m13);
  // t_free(m14);
  // t_free(m15);
  // t_free(m16);
  // t_free(m17);
  // t_free(m18);
  // t_free(m19);
  // t_free(m20);
  // t_free(m21);

    // t_free(m1);
  // t_free(m2);
  // t_free(m3);
  // t_free(m4);
  // t_free(m5);
  // t_free(m6);
  
  //void * finalCheck  = t_malloc(50);


  
  // void * m1  = t_malloc(50);
  // t_free(m1);

  // void*m1_2 = t_malloc(50);
  // t_free(m1_2);


  // void * m2  = t_malloc(300);
  //   t_free(m2);

  // void * m3  = t_malloc(30);


  // void * m4  = t_malloc(50);
  // t_free(m3);
  // t_free(m4);


  // void * m5 = t_malloc(50);

  // t_free(m5);


  // t_malloc(50);
  // t_malloc(50);
  // t_malloc(60);
  // void * m2  = t_malloc(38);
  // t_malloc(593);
  // t_malloc(399);

  // void * m3  = t_malloc(555);
  // t_free(m2);
  //void * m3  = t_malloc(400);


  //pt4 reporting:
  // void* array[24];
  // int i = 0;

  // for(size_t size = 1; size <= 8 * 1024 * 1024; size *= 2){
  //   // startTime = clock();
  //   void* makeMe = t_malloc(size); 
  //   array[i] = makeMe;
  //   i++;
  //   // t_free(makeMe);
  //   // endTime = clock();
  //   // printf("%f \n", (double)(endTime - startTime) / CLOCKS_PER_SEC);
  // }

  // for(int i = 0; i<24; i++){
  //   startTime = clock();
  //   t_free(array[i]);
  //   endTime = clock();
  //   printf("%f \n", (double)(endTime - startTime) / CLOCKS_PER_SEC);
  // }

//memutil reporting

//random mallocs and frees

  // startTime = clock();
  // endTime = clock();
  // printf("%f \n", (double)(endTime - startTime) / CLOCKS_PER_SEC);

  t_malloc(1000);
  void * m5  = t_malloc(200);
  void * m7  = t_malloc(495);  
  void * m3  = t_malloc(120);  
  t_free(m5);
  t_free(m7);
  void * m2  = t_malloc(500);
  void * m6  = t_malloc(300);
  void * m9  = t_malloc(65);
  t_free(m9);
  void * m10  = t_malloc(500);
  void * m8  = t_malloc(300);
  t_free(m10);
  void * m4  = t_malloc(1000);
  void * m11  = t_malloc(200);
  t_free(m2);
  t_free(m3);
  t_free(m6);
  void * m1  = t_malloc(1000);
  t_free(m8);
  t_free(m1);
  t_free(m4);
  t_free(m11);
  
// // //mmff
//   void * m3_1  = t_malloc(4000);
//   void * m5_1  = t_malloc(63);
//   void * m1_1  = t_malloc(1000);
//   void * m4_1  = t_malloc(125);
//   void * m11_1  = t_malloc(1);
//   void * m2_1  = t_malloc(500);
//   void * m6_1  = t_malloc(32);
//   void * m9_1  = t_malloc(4);
//   void * m7_1  = t_malloc(16);  
//   void * m10_1  = t_malloc(2);
//   void * m8_1 = t_malloc(8);

//   void * m3_2  = t_malloc(250);
//   void * m5_2  = t_malloc(63);
//   void * m1_2  = t_malloc(1000);
//   void * m4_2  = t_malloc(125);
//   void * m11_2  = t_malloc(1);
//   void * m2_2 = t_malloc(500);
//   void * m6_2  = t_malloc(32);
//   void * m9_2  = t_malloc(4);
//   void * m7_2  = t_malloc(16);  
//   void * m10_2  = t_malloc(2);
//   void * m8_2 = t_malloc(8);
 
//   t_free(m5_1);
//   t_free(m6_1);
//   t_free(m7_1);
//   t_free(m9_1);
//   t_free(m10_1);
//   t_free(m2_1);
//   t_free(m3_1);
//   t_free(m8_1);
//   t_free(m1_1);
//   t_free(m4_1);
//   t_free(m11_1);

//   t_free(m5_2);
//   t_free(m6_2);
//   t_free(m7_2);
//   t_free(m9_2);
//   t_free(m10_2);
//   t_free(m2_2);
//   t_free(m3_2);
//   t_free(m8_2);
//   t_free(m1_2);
//   t_free(m4_2);
//   t_free(m11_2);




  

  return 0;
}