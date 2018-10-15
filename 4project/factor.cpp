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

int main(int argc, char *argv[])
{
   // ensure you receive proper arguments/input
   if (argc > 2) {
      cerr << "error: impropery usage" << endl;
      cerr << "syntax: " << argv[0] << endl;
      cerr << "or" <<endl;
      cerr << "syntax: " << argv[0] << " <equation>" << endl;
      exit(1);
   }
   // Vars to get information from maxima
   int fdToMax[2],fdFromMax[2], status;
   pipe(fdToMax);
   pipe(fdFromMax);
   pid_t pid = fork();
   string userInput = "";
   char programOutput[300];
   string totalOutput = "";
   
   // change file descriptors of child to talk to maxima
   if (pid == 0){
	dup2(fdToMax[0], 0);
	dup2(fdFromMax[1], 1);
	close(fdToMax[0]);
        close(fdFromMax[0]);
	close(fdToMax[1]);
	close(fdFromMax[1]);
	char *args[] = {"maxima", "-q", NULL};
	execvp("maxima", args);
   }
   //change output
   string format = "display2d: false;\n";
   write(fdToMax[1], format.c_str(), format.length());

   // throw away first read
   char trash[100];
   read(fdFromMax[0], trash, sizeof(trash)-1);
   read(fdFromMax[0], trash, sizeof(trash)-1);

   // don't quit untill user wants to quit
   while(1){ 
   
   // prompt input 
   cout << "> ";
   userInput = "";
   if (argc == 1) 
     cin >> userInput;
   else if(argc == 2)
     userInput  = argv[1];

   if(userInput == "quit")
     break;
   cout << userInput << " = ";
   
   //write their input to maxima
   userInput = "factor("+userInput+");\n";
   int bytes = userInput.length();
   write(fdToMax[1], userInput.c_str(), bytes);
   sleep(1);

   // throw away garbage prompt output
   char trash[100];
   read(fdFromMax[0], trash, 25);

   // read maxima prompt
   int complete = 0;
   while(!complete){
     int nread = read(fdFromMax[0], programOutput, sizeof(programOutput)-1);
     programOutput[nread] = '\0';
     int size = strlen(programOutput);
     if(strchr(programOutput,'%') != NULL)
	complete = 1;
     // find out where the (%o ) is and don't print that
     
     cout << programOutput << endl;
   }
   if (argc ==2)
     break;
   }
   // close maxima
   string quitMessage = "quit();\n";
   int bytes = quitMessage.length();
   write(fdToMax[1], quitMessage.c_str(), bytes);
   
   // wait on child
   wait(&status); 
   exit(0);
}

