#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>


#define SHMSZ     1024

int main()
{
    int shmid;
    key_t key;
    char *shm, *s;

    /*
     * We need to get the segment named
     * "5678", created by the server.
     */
    key = 5119;
    /*
     * Locate the segment.
     */
    if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }


    /*
     * Now we attach the segment to our data space.
     */

    shm = (char*) shmat(shmid, NULL, 0);
    
    

    /*
     * Now read what the server put in the memory.
     */
     
    for (s = shm; *s != 0; s++)
        putchar(*s);
    putchar('\n');
    
    shmdt(s);

//Message sent to the server
 printf("Enter a message: ");
  scanf("%s", s);
  
  printf("Data written in memory: %s\n", s);
 


    /*
     * Finally, change the first character of the 
     * segment to '*', indicating we have read 
     * the segment.
     */
    *shm = '*';

   
   
}

