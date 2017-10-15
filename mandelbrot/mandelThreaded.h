#include <stdio.h>
#include <stdlib.h>


struct task {
  mandel_Pars *pars;//param
  int *res;//param
  int maxIterations;//param
  pthread_t tid;
  int status;
};
typedef struct task taskT;

extern int init_threads(int NUM_OF_THREADS) ;

extern void *waitUntilGetTask(void *newtask);

extern int find_next_finished_thread(int numOfThreads);

extern int collectResults(int numOfThreads); 
