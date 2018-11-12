/*  Make the necessary includes and set up the variables.  */

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <stdlib.h>
#include <netdb.h>

using namespace std;

int main(int argc, char *argv[])
{
    int sockfd, port, len;
    struct addrinfo h, *r;
    int result, n;
    string input, response;
    char buffer[80];

    if (argc != 3) {
       cerr << "usage: " << argv[0] << " IPADDR PORT" << endl;
       exit(1);
    }

    getaddrinfo(argv[1], argv[2], NULL, &r);

    // Create socket for clietn
    sockfd = socket(r->ai_family, r->ai_socktype, r->ai_protocol);

    // Connect our socket to the server's socket.  */
    result = connect(sockfd, r->ai_addr, r->ai_addrlen);

    if(result == -1) {
        cerr << "oops: client1";
        exit(1);
    }

    // We can now read/write via sockfd.  */
    cout << " > ";
    while (getline(cin,input)) {
      send(sockfd, input.c_str(), input.length()+1,0);
      response.clear();
      while (true) {
	n = recv(sockfd, buffer, sizeof(buffer), 0);
	if (buffer[n-1] == '\0') {
	  response.append(buffer);
	  break;
	}
	else
	  response.append(buffer, n);
      }
      cout << response << endl;
      cout << " > ";
    }

    cout << "Client is closing socket." << endl;
    close(sockfd);
    exit(0);
}
