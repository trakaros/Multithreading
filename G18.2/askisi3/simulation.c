#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include "myBinSema.h"

int program_counter = 0;

int main( int argc, char *argv[] )  {
  int fd;//control file descriptor
  int i;
  char buffer[3];
  int curr_time = 5;
  /*
  open file including the arrival times
  */  
  fd = open("control_file.txt", O_RDONLY, S_IRWXU);
  if (fd < 0) {
    printf("Error opening control file. Exiting\n");
    return -1;
  }
  
  while (1)  {
    i = -1;
    do {
      i++;
      read(fd, buffer+i, sizeof(char) );  
    } while (buffer[i] != '\n');
  
    if ( atoi(buffer) > curr_time ) { // if time arrival is bigger than the current time
      buffer[i] = '\0';
      printf("curr_time: %d seconds\n", curr_time);
      printf("next passenger: %d seconds\n", atoi(buffer) );
      lseek(fd, -( strlen(buffer)+1 ), SEEK_CUR );
      
      //break;
    }
    else { // a new passenger now can go to the entrance, create a thread for him
    
    
    
    
    }
  }
  buffer[i] = '\0';
  printf("last buffer is %d\n", atoi(buffer) );
  
  
  return 0;
}