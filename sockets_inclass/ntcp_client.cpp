/*  Make the necessary includes and set up the variables.  */

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    int sockfd, port, len;
    struct sockaddr_in address;
    struct sockaddr *p_addr = (struct sockaddr *)&address;
    int result, n;
    string input, response;
    char buffer[80];

    if (argc != 3) {
       cerr << "usage: " << argv[0] << " IPADDR PORT" << endl;
       exit(1);
    }

    port = atoi(argv[2]);

    // Create a socket for the client.
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Create the server socket address
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(argv[1]);
    len = sizeof(address);

    // Connect our socket to the server's socket.  */
    result = connect(sockfd, p_addr, len);
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
