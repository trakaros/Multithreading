#include <pthread.h>
#include <stdio.h>


int my_down(pthread_mutex_t *mutex);


int my_init(pthread_mutex_t *mutex, int val)  {
  pthread_mutexattr_t myattr;
  
  if (pthread_mutexattr_init(&myattr) != 0 ) {
    printf("Error at initializing the attributes. Exiting\n");
    return -1;
  }
 
  if (pthread_mutexattr_settype(&myattr,  PTHREAD_MUTEX_NORMAL) != 0 ) {
    printf("Error at initializing the attributes. Exiting\n");
    return -1;
  }

  if (pthread_mutex_init(mutex, &myattr) != 0) {
    printf("Error at creating mutex. Exiting\n");  
    return -1;
  }
/*
  if (pthread_mutex_init(mutex, NULL) != 0) {
    printf("Error at creating mutex. Exiting\n");  
    return -1;
  }
*/

  if (val == 0)  {
    if (my_down(mutex) != 0)
      return -1;
     
  }     
  return 0;
}

int my_up(pthread_mutex_t *mutex)  {
  if ( pthread_mutex_unlock(mutex) == -1)  {
    printf("Error unlocking the mutex. Exiting\n");
    return -1;
  }

  return 0;
}


int my_down(pthread_mutex_t *mutex) {
  if ( pthread_mutex_lock(mutex) == -1)  {
    printf("Error locking the mutex. Exiting\n");
    return -1;
  }
  
  return 0;
}

int my_destroy(pthread_mutex_t *mutex) {

  if ( pthread_mutex_destroy(mutex) != 0)  {
    printf("Error destroying the mutex. Exiting\n");
    return -1;
  }
  
  return 0;
}
