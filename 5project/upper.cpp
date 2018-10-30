#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>


#define TEXT_SZ 256
struct shared_use_st {
    int written_by_rev;
    char some_text[TEXT_SZ];
};


using namespace std;

int main(int argc, char * argv[])
{
  char message[256];

  // create shared memory space
  void *shared_memory = (void *)0;
  struct shared_use_st *shared_stuff;
  int shmfd;
  // Open a shared memory object
  shmfd = shm_open(argv[2], O_RDWR, 0);
  if (shmfd == -1) {
    perror("shm_open");
    exit(1);
  }
  // Map the object into our address space.
  shared_memory = mmap(NULL, sizeof(struct shared_use_st),PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
  if(shared_memory == MAP_FAILED) {
    perror("mmap");
    exit(1);
  }
  close(shmfd);
  shared_stuff = (struct shared_use_st *)shared_memory;

  // acquire the semaphore
  sem_t *sem_p;
  char const *sem_name = argv[1];
  sem_p = sem_open(sem_name, O_CREAT | O_EXCL, 0600, 1);
  
  // check to see if reverse has written to it yet 
  while(1){
  sem_wait(sem_p);
  if(shared_stuff->written_by_rev == 1){
    sem_post(sem_p);
    break;
  }
  else{
    sem_post(sem_p);
    sleep(1);
  }
  }

  sem_wait(sem_p); 
  // get string from shared memory
  strncpy(message, shared_stuff->some_text, TEXT_SZ);

  // capitolize everything
  locale loc;
  for (size_t i=0; i <strlen(message); ++i)
    message[i] = toupper(message[i],loc);
  message[256-1] = '\0';

  // write to shared memory
  strncpy(shared_stuff->some_text, message, TEXT_SZ); 

  // change back the rev written for next time
  shared_stuff->written_by_rev = 0;

  // release the semaphore
  sem_post(sem_p);
  sem_close(sem_p);
  sem_unlink(sem_name);

  // kill shared memory
  munmap(shared_memory, sizeof(struct shared_use_st));

  return 0;
}
