#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <netdb.h>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

int main(int argc, char *argv[])
{
    int sockfd;
    int len;
    struct addrinfo *r;
    int nread, result;

    if(argc != 3) {
       cerr << "syntax: ./client server_hostname listening_port" << endl << endl;
       return 1;
    }
    // use getaddrinfo for ease
    getaddrinfo(argv[1], argv[2], NULL, &r);

    // use r datastructure to make socket
    sockfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol);

    // make connection
    result = connect(sockfd, r->ai_addr, r->ai_addrlen);
    if(result == -1) {
        cerr << "the client failed to connect to the server" << endl;
        return 1;
    }
    string userInput;
    static char buffer[256];
    int buflen = sizeof(buffer);
    
    userInput = "READY";
    write(sockfd, userInput.c_str(), userInput.length()+1);
    int ready = 0;

    while(1) {
      // listen for GO
      nread = read(sockfd, buffer, buflen);
      // clt+d was pressed end the program
      if (userInput.length() == 0){
        cout << "exiting" << endl;
        break;
      }
      if(strcmp(buffer,"STOP")==0){
	// read the scoreboard from the server
	cout << "Programing stopping because the other player left" << endl;
        break;
      }
      else if(strcmp(buffer,"GO")==0)
        ready = 1;

      // tell the user which player they are
      nread = read(sockfd, buffer, buflen);
      if(strcmp(buffer,"1") == 0)
        cout << "You are player 1." << endl;
      else
        cout << "You are player 2." << endl;       

      // playing the game with the computer
      if(ready){
	while(1){
	  // enter in your move
	  do{
	    // 0=exit, 1=rock, 2=paper, 3=scissors
	    cout << "0: Exit" << endl;
	    cout << "1: Rock" << endl;
	    cout << "2: Paper" << endl;
	    cout << "3: Scissors" << endl;
	    cout << "Enter Choice: ";
	    cin >> userInput;
	    cout << endl;
	  }while( (userInput!="0") && (userInput!="1") && (userInput!="2") && (userInput!="3") );
	 // write move to server
         write(sockfd, userInput.c_str(), userInput.length()+1);
	 
	 // listen for their results
         nread = read(sockfd, buffer, buflen);

	 // see if someone wanted to stop
	 if (strcmp(buffer,"STOP") == 0){
	    nread = read(sockfd, buffer, buflen);
	    string p1_score = buffer;
            nread = read(sockfd, buffer, buflen);
            string p2_score = buffer;
	    cout << "Game has ended" << endl;
	    cout << "Final Score" << endl;
	    cout << "Player 1: " << p1_score << endl;
	    cout << "Player 2: " << p2_score << endl;

	    return 0;
	 }

	 // print out the wniner
         cout << buffer << endl;
	}
      }
  }  
  
  close(sockfd);
  return 0;
}
