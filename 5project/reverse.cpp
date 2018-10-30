#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>
#include <sys/ipc.h> 
#include <sys/msg.h>
#include <cstdlib>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define PMODE 0666

// shared memory
#define TEXT_SZ 256
struct shared_use_st {
    int written_by_rev;
    char some_text[TEXT_SZ];
};


using namespace std;

int main(int argc, char* argv[])
{
  char message[256];

  // create shared memory space
  int running = 1;
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

  // receive message
  int i;
  mqd_t mqfd;
  struct mq_attr attr;
  int open_flags = 0;
  attr.mq_maxmsg = 256;
  attr.mq_msgsize = 256;
  attr.mq_flags   = 0;
  open_flags = O_RDONLY|O_CREAT;
  mqfd = mq_open(argv[3],open_flags,PMODE,&attr);
  mq_receive(mqfd,message,sizeof(message),NULL);

  // reverse message
  int n = strlen(message); 
  for(int i = 0; i < n/2; i++)
    swap(message[i], message[n-i-1]);
  message[n] = '\0';
  
  // acquire the semaphore
  sem_t *sem_p;
  char const *sem_name = argv[1];
  sem_p = sem_open(sem_name, O_CREAT | O_EXCL, 0600, 1);
  sem_wait(sem_p);

  // write to shared memory object
  strncpy(shared_stuff->some_text, message, TEXT_SZ);
  shared_stuff->written_by_rev = 1;

  // release the semaphore
  sem_post(sem_p);
  sem_close(sem_p);
  sem_unlink(sem_name);

  // unmap
  munmap(shared_memory, sizeof(struct shared_use_st));

  return 0;
}
