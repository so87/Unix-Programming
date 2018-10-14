#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <algorithm>
#include <signal.h>
#include <iostream>
#include <string.h>
#include <sys/wait.h>

using namespace std;


// "wtr_xampl2.cpp" gives examples of forking and piping information
// dup and dup2 help read/write information to other programs via standard input/output

int main(int argc, char *argv[])
{
   // ensure you receive proper arguments/input
   if (argc != 1) {
      cerr << "error: impropery usage" << endl;
      cerr << "syntax: " << argv[0] << endl;
      exit(1);
   }
   // Vars to get information from maxima
   int fd[2], status;
   pipe(fd);
   pid_t pid = fork();
   string userInput = "";
   char programOutput[300];
   string totalOutput = "";

   // change file descriptors of child to talk to maxima
   if (pid == 0){
	dup2(fd[0], 0);
	dup2(fd[1], 1);
	close(fd[0]);
	close(fd[1]);
	char *args[] = {"maxima", "-q", NULL};
	execvp("maxima", args);
   }

   // keep using maxima until user wants to quit
   while(userInput != "quit"){
     // get input from user
     if(pid != 0){
	cout << "Input an equation to factor: ";
        cin >> userInput;
	cout << endl;
     }
     // make the child send that input to maxima
     if((pid == 0) && (userInput != "quit")){
	//write to pipe with user's input
	int bytes = userInput.length();
	write(fd[1], userInput.c_str(), bytes);

	while(1){
	  // get output from maxima until we see that character
	  read(fd[0], programOutput, sizeof(programOutput)-1);
	  totalOutput += programOutput;
	  // this character is sent when maxima is done
	  if(strchr(programOutput,'%') != NULL)
	    break;
	}
	exit(3);
	cout << "wwooo child made it past break" << endl;
     }
     // display that back to user
     if(pid != 0){
        printf("From read: %s\n", totalOutput);
     }
     totalOutput = "";
   }
   // make sure all processes are closed down properly
   string quitMessage = "quit();";
   if (pid == 0){
	int bytes = quitMessage.length();
	write(fd[1], quitMessage.c_str(), bytes);
   }
  
   cout << pid << " finished" << endl;
   waitpid(pid, &status, 0); 
   exit(0);
}

