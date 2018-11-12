#include <unistd.h>
#include <netdb.h>
#include <fstream>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char *argv[])
{
    int sockfd;
    int len;
    struct addrinfo *r;
    int nread, result;

    if(argc != 3) {
       cerr << "syntax: ./shout server_hostname listening_port" << endl << endl;
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

    string UserInput, response;
    static char buffer[256];
    int buflen = sizeof(buffer);

    string inputName, outputName;
    cout << "Connected to server!" << endl;
    cout<<"Enter input and output file names: ";
    
    while(cin>>inputName) { 
      cin>>outputName;
 
      // clt+d was pressed end the program
      if (inputName.length() == 0){
	cout << "exiting" << endl;
        break;
      }
      // the user wants to open files
      ifstream inFile(inputName.c_str());
      ofstream outFile(outputName.c_str());

      while(!inFile)
      {
        cout<<"Error opening input file "<< inputName <<endl;
        cout<<"Enter input and output file names: " << endl; 
        cin>>inputName; 
        inFile.open(inputName.c_str());   
      }
      while(!outFile)
      {
        cout<<"Error opening output file "<< outputName <<endl;
        cout<<"Enter input and output file names: " << endl;
        cin>>outputName;
        outFile.open(outputName.c_str());
      }

      while(getline(inFile, UserInput))
      {
        write(sockfd, UserInput.c_str(), UserInput.length()+1);
        nread = read(sockfd, buffer, buflen);
        outFile << buffer << endl;
      }
      cout << "Complete" << endl;
      cout << "Enter input and output file names: ";

      inFile.close();
      outFile.close();
    }

    // close the connection and return successfully
    close(sockfd);
    return 0;
}
