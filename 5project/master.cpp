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
const int MAXLEN = 512;

enum msgtype { SRVMSG=1, CLIMSG };

struct msg { long type; char data[MAXLEN]; };


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
  msg srvmsg, rcdmsg;
  string srvstr, rcdstr;
  time_t curtime;
  srvmsg.type = SRVMSG;
  string stoc("/myq_stoc"), ctos("/myq_ctos");
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 1;
  attr.mq_msgsize = sizeof(srvmsg);
  mqd_t stoc_id = mq_open(stoc.c_str(),O_CREAT | O_EXCL | O_WRONLY,0600,&attr);
  attr.mq_msgsize = sizeof(rcdmsg);
  mqd_t ctos_id = mq_open(ctos.c_str(),O_CREAT | O_EXCL | O_RDONLY,0600,&attr); 
  
  // create children
  int pid1,pid2,pidParent;
  int status;

  // loop until clt+D
  while(1){ 
    // get input from user
    string userInput = "";
    cout << "> "; 
    cin >> userInput;
    char message[256];
    strcpy(message,userInput.c_str());

    // clt+d was pressed end the program
    if (userInput.length() == 0)
      return 1;

    // send message to IPC
    

    // children manipulate the strings 
    pid1 = fork();
    if(pid1 == 0){
      // two children executing reverse and upper programs
      pid2 = fork();
      if(pid2==0){
        // reverse
        execl("reverse", "reverse",&sem_name, message, (char *)0);
      }
      else{
        // upper
        execl("upper", "upper",&sem_name, message, (char *)0);
      }
    }
    // wait for children to finish manipulating message
    else{
      wait(&status);
      // look at shared memory to see the resulting string
      
      
      cout << endl;
      cout << "Message: " << message << endl;
    }
  }

  return 0;
}
