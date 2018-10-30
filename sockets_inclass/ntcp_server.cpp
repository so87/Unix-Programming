/*  Make the necessary includes and set up the variables.  */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>

using namespace std;

int main()
{
    int read_count, s_sfd, c_sfd;
    struct sockaddr_storage st_addr;
    struct sockaddr_in *s_addr, *c_addr;
    socklen_t st_len, s_len, c_len;
    struct addrinfo hints, *res;
    bool shutdown = false;
    char buffer[80];
    string msg;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Use port 0 for dynamic assignment
    getaddrinfo(NULL, "0", &hints, &res);

    // Make a socket
    s_sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    // Name the socket
    bind(s_sfd, res->ai_addr, res->ai_addrlen);    

    // Find out what port we've been assigned.
    // Note that I am assuming we are bound to an IPV4 address
    // Although getsockname() will work with either IPV4 or IPV6
    // I have to cast the return address to the appropriate type
    st_len = sizeof(st_addr);
    getsockname(s_sfd, (struct sockaddr *)&st_addr, &st_len);
    if(st_addr.ss_family == AF_INET) {
      s_addr = (struct sockaddr_in *)&st_addr;
      cout << "IP: " << inet_ntoa(s_addr->sin_addr)
	   << "\t\tPort: " << ntohs(s_addr->sin_port) << endl;
    }
    else {
      cout << "Not bound to an IPV4 port" << endl;
      exit(-1);
    }

    // Create a connection queue and wait for clients.
    listen(s_sfd, 5);
    while(1) {
        cout << "server waiting" << endl;
	// Accept a connection.
        c_len = sizeof(c_addr);
        c_sfd = accept(s_sfd, (struct sockaddr *)&c_addr, &c_len);
	cout << "client is connected" << endl;
	//  We can now read/write to client on client_sockfd.
        while(1) {
	  msg.clear();
	  // read loop - read until null byte is received.
	  while(true) {
	    read_count = recv(c_sfd, buffer, sizeof(buffer), 0);
	    if(read_count == 0)
	      break;
	    if(buffer[read_count-1] == '\0') {
	      msg.append(buffer);
	      break;
	    }
	    else
	      msg.append(buffer,read_count);
	  }  // read loop
	  if(read_count == 0)
	    break;
	  cout << "Server: " << msg << endl;
	  reverse(msg.begin(), msg.end());
          send(c_sfd, msg.c_str(), msg.length()+1 , 0);
	  if(msg.compare("nwodtuhs")==0) {
	    shutdown = true;
	    break;
	  }
        } // client loop
        cout << "Server is closing socket." << endl;
        close(c_sfd);
	if(shutdown)
	  break;
    }
    cout << "server is shutting down." << endl;
    close(s_sfd);
}

