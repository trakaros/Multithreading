struct task {
  mandel_Pars *pars;//param
  int *res;//param
  int maxIterations;//param
  pthread_t tid;
  //pthread_mutex_t  sem;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  volatile int condVal;   
};
typedef struct task taskT;

extern int init_threads(int numofslices) ;

extern void *waitUntilGetTask(void *numOfThread);

extern int find_next_finished_thread(int numOfThreads);
