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


using namespace std;

int main(int argc, char* argv[])
{
  //int key = atoi(argv[2]);
  string userInput = argv[2];
  char message[256];
  strcpy(message,userInput.c_str());

  // acquire the semaphore
  sem_t *sem_p;
  char const *sem_name = argv[1];
  sem_p = sem_open(sem_name, O_CREAT | O_EXCL, 0600, 1);
  sem_wait(sem_p);

  // receive message
  //int msgid = msgget(key, 0666 | IPC_CREAT);
  //msgrcv(msgid, &message, sizeof(message), 1, 0);
  
  // reverse message
  int n = strlen(message); 
  for(int i = 0; i < n/2; i++)
    swap(message[i], message[n-i-1]);
  message[n] = '\0';

  cout << message << endl;
  // send message
  //msgsnd(msgid, &message, sizeof(message), 0);
  
  // release the semaphore
  sem_post(sem_p);
  sem_close(sem_p);
  sem_unlink(sem_name);

  return 0;
}
