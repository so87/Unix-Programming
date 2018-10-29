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


using namespace std;

// IPC message
#define PMODE 0666

int main(int argc, char * argv[])
{
  // correct args
  if (argc > 1) {
      cerr << "error: improper usage" << endl;
      cerr << "syntax: " << argv[0] << endl;
      exit(1);
   }

  // create semaphore
  char const *sem_name = "/cs";

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

  // create children
  int pid1,pid2,pidParent;
  int status;

  // loop until clt+D
  while(1){ 
    // get input from user
    string userInput = "";
    cout << "> "; 
    cin >> userInput;
    char message[128];
    strcpy(message,userInput.c_str());

    // clt+d was pressed end the program
    if (userInput.length() == 0){
    // destroy the message queue
    if (mq_close(mqfd) == -1)
      perror("mq_close failure on mqfd");
    if (mq_unlink("myipc") == -1)
      perror("mq_unlink failure in test_ipc");
      return 1;
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
        execl("reverse", "reverse",&sem_name, message_queue_name.c_str(), (char *)0);
      }
      else{
        // upper
        execl("upper", "upper",&sem_name, message, (char *)0);
      }
    }
    
    // wait for children to finish manipulating message
    else{
      wait(&status);
      // look at shared memory and print the output
      cout << endl;
      cout << "Message: " << message << endl;
    }
  }
  // close IPC
  if (mq_close(mqfd) == -1)
    perror("mq_close failure on mqfd");


  return 0;
}
