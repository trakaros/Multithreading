#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 10000


/* struct quickArgsT
- it contains the arguments passed to the next recursive quicksort 

-  left and right are the new array limits
*/
struct quickArgs {
  int left;
  int right; 
};
typedef struct quickArgs quickArgsT;

/* struct taskT
- each task has its threadID, the number of childs it has, a pointer
to its parent task and a void pointer that passes argument to new quicksort
- a parent can have <= 2 childs(each child is a recursive call to quicksort,
think it as an unbalanced binary tree)
*/

struct task {
  pthread_t tid;
  quickArgsT *args;
  int childThreads;
  struct task *parent;
};
typedef struct task taskT;  

int array[MAX_SIZE];


/*
- the initial task struct
- there is no linked list, each node is only connected to
its father and is freed recursively at main
*/
taskT *taskHead = NULL;


/* createTask()
- called by myquicksort
- create a node and assign him the left or right recursive 
step of quicksort.
- connect the node to its father, initialize its childs to 
zero and get the new array limits
*/
taskT *createTask(taskT *parent, int left, int right)  {
  quickArgsT *newArgs = NULL;
  taskT *newtask = NULL;
  
  newArgs = (quickArgsT *)malloc( sizeof(quickArgsT) );
  if (newArgs == NULL)  {
    printf("Error allocating args memory\n");
    return NULL;
  } 
  newArgs->left = left;
  newArgs->right = right;
  
  newtask = (taskT *)malloc( sizeof(taskT) );
  if (newtask == NULL)  {
    printf("Error allocating task memory\n");
    return NULL;
  }    
  newtask->args = newArgs;
  newtask->childThreads = 0;
  newtask->parent = parent;

  return newtask;
}


void *myquicksort(void *nextTask) {
  
  taskT *mytask = (taskT *)nextTask;
  taskT *rTask = NULL;
  taskT *lTask = NULL;
   
  //init counters
  int i = mytask->args->left-1;//left-1;
  int j = mytask->args->right;
  int o = array[mytask->args->right];
  int temp;
        
  while (1)  {
    while ( array[++i] < o )  {    
    }  
    
    while( o < array[--j] )  {
    }
    
    if (i >= j)
      break;
      
    temp = array[i];
    array[i] = array[j];
    array[j] = temp;  
  }
  
  temp = array[i];
  array[i] = array[mytask->args->right];
  array[mytask->args->right] = temp;

   
  /*
  - if i must do left-recursive quicksort, create a new thread
  , assign the arguments and increment the current task's childs
  once
  */
  if (mytask->args->left < i-1)  {
   
    mytask->childThreads++;    
    lTask = createTask(mytask, mytask->args->left, i-1);
    if (lTask == NULL) {
      return NULL;
    }
    
     
    if (pthread_create( &(lTask->tid), NULL, (void *)myquicksort, (void *)lTask) != 0)  {     
      printf("Error creating left thread. Exiting\n");
      return NULL;
    }
  }
  
  /*
  - the same process repeated for the right part of the array
  */    
  if (i+1 < mytask->args->right)  {
  
    mytask->childThreads++;
    rTask = createTask(mytask, i+1, mytask->args->right);
    if ( rTask == NULL) {
      return NULL;
    }

     
 
    if ( pthread_create( &( rTask->tid), NULL, (void *)myquicksort, (void *) rTask) != 0 ) {
      printf("Error creating right thread. Exiting\n");
      return NULL;
    }
      
  }


  /*
  - Wait until all of the childs have stopped working
  */
  while ( mytask->childThreads != 0) {
    
    if (sched_yield() != 0) {
      printf("Error at CPU relinquish. Exiting\n");
      return NULL;
    }
  }

  
  /*
  - extra code in case of the 1st task, because it
  doesnt have a parent
  */
  if (mytask != taskHead)
    mytask->parent->childThreads--;
  
  
  /*
  - free all dynamicaly allocated memory
  */
  free( mytask->args );
  free( mytask );

  return NULL;
}


/* read_number()
- read chars, eliminate ' ' characters and return as an integer 
*/
int read_number()  {
  int i = 0;
  char number_str[20];

  while (1) {  
    number_str[i] = getchar();
    if (number_str[i] == ' ')
      continue;
    else if (number_str[i] == '\n')  {
      number_str[i] = '\0';
      break;
    }
    else
      i++;
  }

   return atoi(number_str);
}


int main(int argc, char *argv[])  {
  quickArgsT *args;
  int i=0, numOfElements;
  
 
  
  printf("Please number of elements<enter>: ");
  numOfElements = read_number();

  //init array
  for (i = 0; i < numOfElements; i++)  {
    printf("no.%d: ", i+1);
    array[i] = read_number();
  } 
  
   
  //init main thread
  taskHead = (taskT *)malloc( sizeof(taskT) );
  if (taskHead == NULL)
    return -1;


  args = (quickArgsT *)malloc( sizeof(quickArgsT) );
  if (args == NULL)  {
    printf("Error allocating main thread. Exiting\n");
    return -1;
  }
  args->left = 0;
  args->right = numOfElements-1;
  taskHead->args = args; 
  taskHead->childThreads = 0;
  taskHead->parent = NULL;
  myquicksort(taskHead);


  for (i = 0; i < numOfElements; i++)  {
    printf("%d, ", array[i]);
  }
  putchar('\n');
  
  return 0;
}
