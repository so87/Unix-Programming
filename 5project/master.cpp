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

using namespace std;

// IPC message
struct mesg_buffer { 
    long mesg_type; 
    char mesg_text[256]; 
} message;

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
  key_t key;
  int msgid;
  int status;
  key = ftok("progfile", 65);
  msgid = msgget(key, 0666 | IPC_CREAT);
  
  // create children
  int pid1,pid2,pidParent;

  // loop until clt+D
  while(1){ 
    // get input from user
    string userInput = "";
    cout << "> "; 
    cin >> userInput;

    // clt+d was pressed end the program
    if (userInput.length() == 0)
      return 1;

    // send message to IPC
    strcpy(message.mesg_text,userInput.c_str());
    msgsnd(msgid, &message, sizeof(message),0); 

    // children manipulate the strings 
    pid1 = fork();
    if(pid1 == 0){
      // two children executing reverse and upper programs
      pid2 = fork();
      if(pid2==0){
        // reverse
        execl("reverse", "reverse",&sem_name, message.mesg_text, (char *)0);
      }
      else{
        // upper
        execl("upper", "upper",&sem_name, message.mesg_text, (char *)0);
      }
    }
    // wait for children to finish manipulating message
    else{
      wait(&status);
      // msgrcv to receive message
      msgrcv(msgid, &message, sizeof(message), 1, 0);
      cout << endl;
      cout << "Message: " << message.mesg_text << endl;
    }
  }

  // kill the message queue
  msgctl(msgid, IPC_RMID, NULL);

  return 0;
}
