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

#define PMODE 0666

using namespace std;

int main(int argc, char* argv[])
{
  char message[128];

  // acquire the semaphore
  sem_t *sem_p;
  char const *sem_name = argv[1];
  sem_p = sem_open(sem_name, O_CREAT | O_EXCL, 0600, 1);
  sem_wait(sem_p);

  // receive message
  int i;
  mqd_t mqfd;
  struct mq_attr attr;
  int open_flags = 0;
  attr.mq_maxmsg = 128;
  attr.mq_msgsize = 128;
  attr.mq_flags   = 0;
  open_flags = O_RDONLY|O_CREAT;
  mqfd = mq_open(argv[2],open_flags,PMODE,&attr);
  mq_receive(mqfd,message,sizeof(message),NULL);

  // reverse message
  int n = strlen(message); 
  for(int i = 0; i < n/2; i++)
    swap(message[i], message[n-i-1]);
  message[n] = '\0';

  cout << message << endl;
  
  // release the semaphore
  sem_post(sem_p);
  sem_close(sem_p);
  sem_unlink(sem_name);

  return 0;
}
