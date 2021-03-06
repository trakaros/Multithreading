#include <pthread.h>
#include "mandelCore.h"
#include "mandelThreaded.h"

#define WORKING 1
#define NOT_WORKING 2
#define DONE 3

taskT *tasks;
 
int init_threads(int numOfThreads)  {
  int i;
  
  //allocate and initialize struct fields
  for (i = 0; i < numOfThreads; i++)  {
    tasks[i].status = NOT_WORKING;

    if ( pthread_create( &tasks[i].tid, NULL, (void *)waitUntilGetTask, (void *)&tasks[i]) != 0) {
      printf("Error creating thread %d. Exiting\n", i);
      return -1;
    }
  }

  return 0;
}



void *waitUntilGetTask(void *newtask)  {    
 // mandel_Calc(&slices[i],maxIterations,&res[i*slices[i].imSteps*slices[i].reSteps]) {

  taskT *slave = (taskT *)newtask;
  while (1)  {
    if ( slave->status == WORKING)  {
      mandel_Calc( slave->pars, slave->maxIterations, slave->res );
      slave->status = DONE;
    }  
  }

 return NULL; 
}
