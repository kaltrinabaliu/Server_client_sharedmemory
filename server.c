#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

int shm_unlock(int shmid)
{
    //Get shared memory segment
    struct shmid_ds *shm_desc = NULL;
    shm_desc = shmat(shmid, 0, 0);

    //Check if segment is locked
    if (shm_desc->shm_perm.mode & SHM_LOCKED)
    {
        //Unlock the segment
        shm_desc->shm_perm.mode &= ~SHM_LOCKED;
        shmctl(shmid, IPC_SET, shm_desc);
    }

    //Detach shared memory segment
    shmdt(shm_desc);

    return 0;
}


int shm_lock(int shmid)
{
    struct shmid_ds shm_lock_data;

    /* Get the current data structure */
    shmctl(shmid, IPC_STAT, &shm_lock_data);

    /* Set the lock */
    shm_lock_data.shm_perm.mode |= SHM_LOCKED;

    /* Set the new mode */
    shmctl(shmid, IPC_SET, &shm_lock_data);
}
#define SHMSZ     1024

int main()
{
    char c;
    int shmid;
    key_t key;
    char *shm, *s;

    /*
     * We'll name our shared memory segment
     * "5678".
     */
    key = 5119;

    /*
     * Create the segment.
     */
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    

    /*
     * Now put some things into the memory for the
     * other process to read.
     */
    s = shm;

 printf("Enter a message: ");
  scanf("%s", s);
  
  printf("Data written in memory: %s\n", s);
  
  sleep(10);
 
 for (s = shm; *s != 0; s++)
        putchar(*s);
    putchar('\n');
  
  
  
    /*
     * Finally, we wait until the other process 
     * changes the first character of our memory
     * to '*', indicating that it has read what 
     * we put there.
     */
    while (*shm != '*')
        sleep(10);
    
    //Locking the shared memory
  shm_lock(shmid);



//Now we can change the first chracter of the segment to something else.
 *shm='#';


//Unlocking the shared memory
shm_unlock(shmid);



//We can now detach and remove the segment.
shmdt(shm);
shmctl(shmid,IPC_RMID, NULL);
}

