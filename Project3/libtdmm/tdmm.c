#include "tdmm.h"
#include <sys/mman.h>
#include <errno.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

alloc_strat_e currStrat;
void* gcCollect;

void* mallocFirstFit(size_t size);
void* mallocBestFit(size_t size);
void* mallocWorstFit(size_t size);
void* mallocBuddyAlloc(size_t size);

typedef struct Node{
  size_t size;
  struct Node* next;
  struct Node* previous;
}Node;


void createNewNode(size_t size);
int checkMSB(size_t num);

void coaleseBefore();
void coaleseAfter();

size_t getAllocTotal();
size_t getRequestsTotal();
size_t getFreeTotal();
int getOverHead();

Node * front;
Node ** currentNode;
Node*current;

void*beginMem;

size_t allocated_memory = 0;
size_t total_available_memory = 0;
size_t totalRequestedMemory = 0; 


clock_t  startTime, endTime;



// TODO: Implement this
void t_init (alloc_strat_e strat, void* stack_bot){
  //assign globals
  currStrat = strat;
  gcCollect = stack_bot;
  
  //declare starting page size
  size_t pageSize = 4096;
  //pointer to the beginning free space
  void* beginMemory = mmap(NULL, pageSize, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0); 
  beginMem = beginMemory;
  front = (Node *) beginMemory;
  front->next = NULL;
  front->previous = NULL;
  front->size = pageSize - 24;
  total_available_memory += pageSize;
  // current = front;

  // Node f;
  // f.size = pageSize - 24;
  // f.previous  = NULL;
  // f.next = NULL;
  // front = &f;
  // current = &f;
}

size_t getAllocTotal(){
  return allocated_memory;
}

size_t getFreeTotal(){
  return total_available_memory;
}

size_t getRequestsTotal(){
  return totalRequestedMemory;
}

void * t_malloc (size_t size){
  totalRequestedMemory += size;
  //implement if size > (pagesize-24) -->4072 
  total_available_memory -= size;
  allocated_memory += size;  
  // printf("M_Total alloc memory: %ld \n", allocated_memory);
  // printf("M_Total free memory: %ld \n", total_available_memory);
  // printf("M_Total Requested Memory: %ld \n", totalRequestedMemory);
  // printf("%f \n", (double)allocated_memory/totalRequestedMemory );
  printf("%ld \n", totalRequestedMemory);
  printf("get total overhead :%d \n", getOverHead());
  startTime = clock();


  if(currStrat == FIRST_FIT){
    return mallocFirstFit(size);
  }
  else if(currStrat == BEST_FIT){
    return mallocBestFit(size);
  }
  else if(currStrat == WORST_FIT){
    return mallocWorstFit(size);
  }
  else if(currStrat == BUDDY){
    return mallocBuddyAlloc(size);
  }
  return NULL;
}

void * mallocFirstFit(size_t size){

  currentNode = &front;
 // printf("first size %ld\n", front->size);

  current = *(currentNode);
  Node * tempCurr = current;


  if(size > 4072){
    void* largePage = mmap(NULL, size*2, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    current = (Node*)largePage;
    current->previous = tempCurr;
    current->next = NULL;
    current->size = size*2 - 24;
    createNewNode(size);
    endTime = clock(); 
    // printf("M_Time passed: %f \n", (double)(endTime - startTime) / CLOCKS_PER_SEC);
    //printf("%f \n", (double)(endTime - startTime) / CLOCKS_PER_SEC);
    return ((void*)current->previous) + 24;
  }
  
  while(current != NULL){

   // printf("cur size before %ld\n", current->size);
    
    if(checkMSB(current->size) == 1){//allocated if msb is 1 
      current = current->next;
      //printf("arrived \n");
    }

    //not allocated
    else{
      //check if can current node can fit size
      //printf("cufrr size %ld \n", current->size);

      if(current->size >= size + 24){ 
        createNewNode(size);
        //printf("added node size %ld \n", current->previous->size);
        endTime = clock(); 
        // printf("M_Time passed: %f \n", (double)(endTime - startTime) / CLOCKS_PER_SEC);
        //printf("%f \n", (double)(endTime - startTime) / CLOCKS_PER_SEC);
        return ((void*)current->previous) + 24;
      }
      //current node cannot fit size
      else{
        current = current->next;
      }
    }
    
  }

  //allocation size requesst too big
  //create new page and continue forward
  int pageSize = 4096;

  void* nextPage = mmap(NULL, pageSize, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  // printf("made it here \n");
  // printf("temp curr size %ld \n", tempCurr->size);
  Node* newPageStart = (Node*) nextPage;
  newPageStart->next = NULL;
  newPageStart->previous = current;
  newPageStart->size = pageSize - 24;
  currentNode = &newPageStart;
  current = *currentNode;
  createNewNode(size);
  
  // printf("M_Time passed: %f \n", (double)(endTime - startTime) / CLOCKS_PER_SEC);
  endTime = clock(); 
  //printf("%f \n", (double)(endTime - startTime) / CLOCKS_PER_SEC);
  return ((void*)current->previous) + 24;
}


int checkMSB(size_t num){
  // printf("we got here %ld \n", num);
  int check = (num >> (63));
  // printf("check val: %d \n", check);
  
  return check & 1;
}


void * mallocBestFit(size_t size){
  
  //printf("alloc req %d \n", size);

  currentNode = &front;
  current = *(currentNode);
  Node * tempCurr = current;


  if(size > 4072){
    void* largePage = mmap(NULL, size*2, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    current = (Node*)largePage;
    current->previous = tempCurr;
    current->next = NULL;
    current->size = size*2 - 24;
    createNewNode(size);
    // endTime = clock(); 
    // printf("%f \n", (double)(endTime - startTime) / CLOCKS_PER_SEC);
    return ((void*)current->previous) + 24;
  }

  //Node * bestFit;
  size_t minSize = __INT_MAX__;
  //printf("hai min size %ld \n", minSize);
  while(current != NULL){

    //printf("cur size before %ld\n", current->size);
    
    
    if(checkMSB(current->size) == 1){//allocated if msb is 1 
      current = current->next;
      //printf("arrived \n");
    }

    //not allocated
    else{
      //check if can current node can fit size
     // printf("cufrr size %ld \n", current->size);

      if(current->size >= size + 24){ 
        //printf("got here\n");
        //printf("pre min size: %ld\n", minSize);
        if(current->size < minSize){
          minSize = current->size;
          //printf("min size: %ld \n", minSize);
          current = current->next;
        }
        else{
          current = current->next;
        }
      }
      //current node cannot fit size
      else{
        current = current->next;
      }
    }
  }

  //allocation size requesst too big
  //create new page and continue forward
  if(minSize == __INT_MAX__){
    int pageSize = 4096;

    void* nextPage = mmap(NULL, pageSize, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    // printf("made it here \n");
    // printf("temp curr size %ld \n", tempCurr->size);
    Node* newPageStart = (Node*) nextPage;
    newPageStart->next = NULL;
    newPageStart->previous = current;
    newPageStart->size = pageSize - 24;
    currentNode = &newPageStart;
    current = *currentNode;
    createNewNode(size);
    // printf("complete \n");
    // endTime = clock(); 
    // printf("%f \n", (double)(endTime - startTime) / CLOCKS_PER_SEC);
    return ((void*)current->previous) + 24;
  }
  //printf("hellooo \n");

  // current = *(currentNode);
  currentNode = &front;
  current = *(currentNode);
  while(current !=  NULL){
    if(current->size == minSize){
      createNewNode(size);
      // endTime = clock(); 
      // printf("%f \n", (double)(endTime - startTime) / CLOCKS_PER_SEC);
      return ((void*)current->previous) + 24;
    }
    else
      current = current->next;
  }
  
}

void * mallocWorstFit(size_t size){
  //printf("alloc req %d \n", size);

  currentNode = &front;
  current = *(currentNode);
  Node * tempCurr = current;


  if(size > 4072){
    void* largePage = mmap(NULL, size*2, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    current = (Node*)largePage;
    current->previous = tempCurr;
    current->next = NULL;
    current->size = size*2 - 24;
    createNewNode(size);
    // endTime = clock(); 
    // printf("%f \n", (double)(endTime - startTime) / CLOCKS_PER_SEC);
    return ((void*)current->previous) + 24;
  }

  //Node * bestFit;
  size_t maxSize = 0;
  //printf("hai min size %ld \n", minSize);
  while(current != NULL){

  //  printf("cur size before %ld\n", current->size);
    
    
    if(checkMSB(current->size) == 1){//allocated if msb is 1 
      current = current->next;
      //printf("arrived \n");
    }

    //not allocated
    else{
      //check if can current node can fit size
      //printf("cufrr size %ld \n", current->size);

      if(current->size >= size + 24){ 
        //printf("got here 1\n");
        //printf("pre min size: %ld\n", maxSize);
        //printf("max size: %ld \n", maxSize);
        if(current->size > maxSize){
          //printf("got here 2\n");
          maxSize = current->size;
          //printf("min size: %ld \n", maxSize);
          current = current->next;
        }
        else{
          current = current->next;
        }
      }
      //current node cannot fit size
      else{
        current = current->next;
      }
    }
    
  }

  //allocation size requesst too big
  //create new page and continue forward
  //printf("max size: %ld", maxSize);
  if(maxSize == 0){
    int pageSize = 4096;

    void* nextPage = mmap(NULL, pageSize, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    // printf("made it here \n");
    // printf("temp curr size %ld \n", tempCurr->size);
    Node* newPageStart = (Node*) nextPage;
    newPageStart->next = NULL;
    newPageStart->previous = current;
    newPageStart->size = pageSize - 24;
    currentNode = &newPageStart;
    current = *currentNode;
    createNewNode(size);
    // printf("complete \n");
    // endTime = clock(); 
    //   printf("%f \n", (double)(endTime - startTime) / CLOCKS_PER_SEC);
    return ((void*)current->previous) + 24;
  }
  //printf("hellooo \n");

  // current = *(currentNode);
  currentNode = &front;
  current = *(currentNode);
  while(current !=  NULL){
    if(current->size == maxSize){
      createNewNode(size);
      // endTime = clock(); 
      // printf("%f \n", (double)(endTime - startTime) / CLOCKS_PER_SEC);
      return ((void*)current->previous) + 24;
    }
    else
      current = current->next;
  }


}

void * mallocBuddyAlloc(size_t size){
  
  //printf("alloc req %d \n", size);

  currentNode = &front;
  current = *(currentNode);
  Node * tempCurr = current;


  if(size > 4072){
    void* largePage = mmap(NULL, size*2, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    current = (Node*)largePage;
    current->previous = tempCurr;
    current->next = NULL;
    current->size = size*2 - 24;
    createNewNode(size);
    // endTime = clock(); 
    //   printf("%f \n", (double)(endTime - startTime) / CLOCKS_PER_SEC);
    return ((void*)current->previous) + 24;
  }

  //Node * bestFit;
  size_t minSize = __INT_MAX__;
  //printf("hai min size %ld \n", minSize);
  while(current != NULL){

  //  printf("cur size before %ld\n", current->size);
    
    
    if(checkMSB(current->size) == 1){//allocated if msb is 1 
      current = current->next;
      //printf("arrived \n");
    }

    //not allocated
    else{
      //check if can current node can fit size
     // printf("cufrr size %ld \n", current->size);

      if(current->size >= size + 24){ 
        //printf("got here\n");
        //printf("pre min size: %ld\n", minSize);
        if(current->size < minSize){
          minSize = current->size;
          //printf("min size: %ld \n", minSize);
          current = current->next;
        }
        else{
          current = current->next;
        }
      }
      //current node cannot fit size
      else{
        current = current->next;
      }
    }
  }

  //allocation size requesst too big
  //create new page and continue forward
  if(minSize == __INT_MAX__){
    int pageSize = 4096;

    void* nextPage = mmap(NULL, pageSize, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    // printf("made it here \n");
    // printf("temp curr size %ld \n", tempCurr->size);
    Node* newPageStart = (Node*) nextPage;
    newPageStart->next = NULL;
    newPageStart->previous = current;
    newPageStart->size = pageSize - 24;
    currentNode = &newPageStart;
    current = *currentNode;
    createNewNode(size);
    // printf("complete \n");
    // endTime = clock(); 
    //   printf("%f \n", (double)(endTime - startTime) / CLOCKS_PER_SEC);
    return ((void*)current->previous) + 24;
  }
  //printf("hellooo \n");

  // current = *(currentNode);
  currentNode = &front;
  current = *(currentNode);
  while(current !=  NULL){
    if(current->size == minSize){
      createNewNode(size);
      // endTime = clock(); 
      // printf("%f \n", (double)(endTime - startTime) / CLOCKS_PER_SEC);
      return ((void*)current->previous) + 24;
    }
    else
      current = current->next;
  }
  
}


void createNewNode(size_t size){
  
  Node* nextNode = (Node*)(((void*)current) + 24 + size);

  nextNode->size = current->size - size - 24;
  nextNode->previous = current;
  nextNode->next = current->next;

  // printf("node size %ld \n", nextNode->size);

  current->size = size;
  // printf("curr size inside %ld \n", current->size);

  current->next = nextNode;
  current->size |= ((size_t)1 << 63); //allocated flag

  current = current->next;
}

void t_free (void *ptr){
  startTime = clock();
  current = (Node*)((void*)ptr - 24);
  size_t s = current->size;
  s <<= 1;
  s >>= 1;
  current->size = s;
  total_available_memory += s;
  allocated_memory -= s;
  coaleseBefore();
  coaleseAfter(); 
  endTime = clock(); 
  //printf("%f \n", (double)(endTime - startTime) / CLOCKS_PER_SEC);
  // printf("M_Total alloc memory: %ld \n", allocated_memory);
  // printf("M_Total free memory: %ld \n", total_available_memory);
  // printf("M_Total Requested Memory: %ld \n", totalRequestedMemory);
  // printf("get total overhead :%d \n", getOverHead());
  // printf("%f \n", (double)allocated_memory/totalRequestedMemory );
  // printf("%f \n", (double)(endTime - startTime) / CLOCKS_PER_SEC);

  // printf("post sizes: %ld \n", current->size); 
}

int getOverHead(){

  int overHead = 0;
  currentNode = &front;
  Node * temp = *(currentNode);
  while(temp != NULL){
    overHead += 24;
  }
  return overHead;
}

void coaleseBefore(){
  //current and currprev
  //printf("curr prev size %ld \n", current->previous->size);

  Node * tempPrev = current->previous;
  if(tempPrev == NULL){
    return;
  }

  void* cur = (void*)current;
  void*adj = (void*) current->previous;
  // printf("prev size: %ld \n", current->previous->size);
  if(cur != (adj + 24 + current->previous->size)){
    // printf("hello \n");
    return;
  }
  

  // printf("prev size %ld \n", tempPrev->size);

  if(checkMSB(tempPrev->size) == 0){
    //printf("hai \n");
    tempPrev->size += current->size + 24;
    tempPrev->next = current->next;
    (current->next)->previous = tempPrev;
    current = tempPrev;
    //printf("curr prev size %ld \n", tempPrev->size);
  }
  else{
    return;
  }
}

void coaleseAfter(){
  //current and currNext
  Node * tempNext = current->next;

  
  if(tempNext == NULL){
    return;
  }

  void *ptr = (void*) current;
  void* cmp = (void*)current->next;

  if(!(cmp == (ptr + 24 + current->size))){
    // printf("hello 2\n");
    return;
  }
  

  //if next is also free
  if(checkMSB(tempNext->size) == 0){
    current->size += tempNext->size + 24;
    current->next = tempNext->next;
    if((tempNext->next) != NULL){
      (tempNext->next)-> previous = current;
    }
  }
  //otherwise nothing happens
  else{
    return;
  }
}

void t_gcollect (void){
  
  void * stackTop;
  stackTop = &gcCollect;


  // TODO: Implement this

  current = *currentNode;
  while(gcCollect != NULL){
    
    while(current != NULL){
      if(checkMSB(current->size) == 1){
        

      }
      
    } 

    gcCollect++;
  }

}