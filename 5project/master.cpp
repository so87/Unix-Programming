#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <sys/ipc.h> 
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <semaphore.h>
#include <sys/types.h>
#include <mqueue.h>
#include <ctime>
#include <sys/mman.h>
#include <sys/wait.h>


using namespace std;

// IPC message
#define PMODE 0666

// shared memory
#define TEXT_SZ 128
struct shared_use_st {
    int written_by_rev;
    char some_text[TEXT_SZ];
};

int main(int argc, char * argv[])
{
  // correct args
  if (argc > 1) {
      cerr << "error: improper usage" << endl;
      cerr << "syntax: " << argv[0] << endl;
      exit(1);
   }

  // create semaphore
  sem_t *sem_p;
  char const *sem_name = "/cs";
  sem_p = sem_open(sem_name, O_CREAT | O_EXCL, 0600, 1);

  // create IPC message queue
  int i;
  int MQstatus = 0;
  mqd_t mqfd;
  struct mq_attr attr;
  int open_flags = 0;
  int num_bytes_to_send;
  int priority_of_msg;
  attr.mq_maxmsg = 128;
  attr.mq_msgsize = 128;
  attr.mq_flags   = 0;
  open_flags = O_WRONLY|O_CREAT;
  string message_queue_name = "myipc";
  mqfd = mq_open(message_queue_name.c_str(),open_flags,PMODE,&attr);
  num_bytes_to_send = 2;
  priority_of_msg = 1;

  // created shared memory segment
  int running = 1; 
  void *shared_memory = (void *)0;
  struct shared_use_st *shared_stuff;
  int shmfd;
  char const *shm_name = "/unixproject5simon";
  shmfd = shm_open(shm_name, O_RDWR | O_CREAT | O_EXCL, 0666);
  // Set the size of the memory object
  if(ftruncate(shmfd, sizeof(struct shared_use_st)) == -1) {
    perror("ftruncate");
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
  shared_stuff->written_by_rev = 0;


  // create children
  int pid1,pid2,pidParent;
  int status;

  // loop until clt+D
  while(1){ 
    // get input from user
    string userInput = "";
    cout << "> "; 
    getline(cin, userInput);
    char message[128];
    strcpy(message,userInput.c_str());

    // clt+d was pressed end the program
    if (userInput.length() == 0){
      break;
    }

    // Generate message to send to reverse
    message[128-1] = '\0';
    mq_send(mqfd,message,sizeof(message),priority_of_msg); 

    // children manipulate the strings 
    pid1 = fork();
    if(pid1 == 0){
      // two children executing reverse and upper programs
      pid2 = fork();
      if(pid2==0){
        // reverse
	string message_queue_name = "myipc";
        execl("reverse", "reverse",&sem_name, shm_name, message_queue_name.c_str(), (char *)0);
      }
      else{
        // upper
        execl("upper", "upper",&sem_name, shm_name, (char *)0);
      }
    }
    
    // wait for children to finish manipulating message
    else{
      wait(&status);
      // look at shared memory and print the output
      sem_wait(sem_p);
      strncpy(message, shared_stuff->some_text, TEXT_SZ); 
      cout << endl;
      cout << "Message: " << message << endl;
      sem_post(sem_p);
    }
  }
  // close IPC
  // destroy the message queue
  if (mq_close(mqfd) == -1)
    perror("mq_close failure on mqfd");
  if (mq_unlink("myipc") == -1)
    perror("mq_unlink failure in test_ipc");
  // destroy shared memory
  munmap(shared_memory, sizeof(struct shared_use_st));
  shm_unlink("/test1");
  //destroy the semaphore
  sem_close(sem_p);
  sem_unlink(sem_name);  

  return 0;
}
