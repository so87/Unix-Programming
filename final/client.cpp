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
    string userInput, response;
    static char buffer[256];
    int buflen = sizeof(buffer);

    string input, output;
    cout << "Connected to server!" << endl;
    cout<<"Please enter your name: ";

    while(cin>>userInput) {
      // clt+d was pressed end the program
      if (userInput.length() == 0){
        cout << "exiting" << endl;
        break;
      }
      if (userInput == "quit"){
	cout << "exiting" << endl;
	break;
      }
      write(sockfd, userInput.c_str(), userInput.length()+1);
      nread = read(sockfd, buffer, buflen);
      cout << buffer << endl;
  }  
  
  close(sockfd);
  return 0;
}
