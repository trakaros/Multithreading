#include "mypipe.h"

volatile int join_threads=0;
extern int fdin;
extern int fdout;
extern volatile int endOfFile;


void *thread_pipereader(void *filename)  {
   int readval;
   
   fdout = open( (char *)filename , O_RDWR | O_CREAT, S_IRUSR ); 
   if (fdout == -1 )  {
     printf("Error opening output file. Exiting\n");
     return NULL;
   }
    
   while(1){
      readval = pipe_read();
      if (readval == 0)  {
        printf("Destroy pipe\n");
        pipe_close();
        join_threads = 1;
        return NULL;
      }
     
   }
}


void pipewriter(char *filename) { 
   int bytesRead;
   char c;
 
   fdin = open( (char *)filename , O_RDONLY, S_IRUSR );
   if (fdin == -1 )  {
     printf("Error opening input file. Exiting\n");
     return ;
   }

   while(1){
     bytesRead = read(fdin, &c, sizeof(char) );
     if (bytesRead == -1)  {
       printf("Read error at writer() function.Exiting\n");
       exit (-1) ; 
     }
     else if (bytesRead == 0) {//Eof 
       endOfFile = 1;
       return;
     }  
     pipe_write(c); 
   }
}


int main(int argc, char **argv)  {
  pthread_t tid1;
  
  pipe_init( atoi(argv[1]) );  
  if (pthread_create(&tid1,NULL,(void *)thread_pipereader,(void *)argv[3]) != 0) {
    printf("Error creating reader thread\n");
    exit(-1);
  }
  pipewriter( argv[2] );  
  
  while (join_threads == 0) {
    printf("poulo again\n");
  }
  /*
  if (pthread_join(tid1,NULL) != 0) {
    printf("error at read thread\n");
    exit(-1);
  }*/
  
  return 0;

}
