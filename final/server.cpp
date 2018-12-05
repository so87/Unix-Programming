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

int main()
{
    // inclass example setup
    int s_sfd;
    struct sockaddr s_addr, c_addr;
    socklen_t s_len, c_len;
    struct addrinfo hints, *res;
    
    fd_set readfds, testfds;
    static char buffer[256];
    int buflen = sizeof(buffer);
    int result;
    string input;
    char host[256];
    char port[10];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    result = getaddrinfo(NULL, "0", &hints, &res);
    if(result == -1){
    cerr << "failed to get addrinfo" << endl;
      return 1;
    }

    s_sfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(s_sfd == -1){
      cerr << "failed to make socket" << endl;
      return 1;
    }
    
    result = bind(s_sfd, res->ai_addr, res->ai_addrlen);    
    if(result == -1){
      cerr << "failed to bind socket" << endl;
      return 1;
    }

    s_len = sizeof(s_addr);
    result = getsockname(s_sfd, &s_addr, &s_len);
    if(result == -1){
      cerr << "failed to get socket name" << endl;
      return 1;
    }

    int flags;
    flags = NI_NUMERICHOST | NI_NUMERICSERV;
    result = getnameinfo(&s_addr, sizeof(s_addr),host, sizeof(host), port, sizeof(port), flags);
    if(result != 0){
      cerr << "failed to make socket" << endl;
      return 1;
    }
    // print that info back to user so client can connect
    cout << "IP: " << host << endl;
    cout << "PORT: " << port << endl;
    // deallocate
    freeaddrinfo(res);
    listen(s_sfd, 10);
    FD_ZERO(&readfds);
    FD_SET(s_sfd, &readfds);

    int maxc = 4;
    int p1_fd = 0;
    int p2_fd = 0;
    int connected = 0;

    while(1) {
        int fd;
        int nread;
        testfds = readfds;

        // select to find the available FD
        result = select(maxc, &testfds, (fd_set *)0, (fd_set *)0, (struct timeval *) 0);
        if(result < 1) {
            cerr << "error in select call" << endl;
            return 1;
        }
        for(fd = 0; fd < maxc; fd++) {
            if(FD_ISSET(fd,&testfds)) {
		// create client file descriptors
                if(fd == s_sfd) {
                    c_len = sizeof(c_addr);
                    if(connected==0){
		      p1_fd = accept(s_sfd, &c_addr, &c_len);
                      FD_SET(p1_fd, &readfds);
                      cout << "player 1 using fd " << p1_fd << endl;
		      connected += 1;
   		      maxc++;
		    }
		    else if(connected==1){
                      p2_fd = accept(s_sfd, &c_addr, &c_len);
                      FD_SET(p2_fd, &readfds);
                      cout << "player 2 using fd " << p2_fd << endl;
                      connected += 1;
                      maxc++;
		      // time to play the game both are connected
		      input = "GO";
		      write(p1_fd, input.c_str(), input.length()+1);
                      write(p2_fd, input.c_str(), input.length()+1); 
                    }
                }	
                else {
		  // flush the input and start over
		  input.clear();
		  while(true) {
                    nread = read(fd, buffer, buflen);
                    if((nread == 0 ) || (buffer == "STOP")){
			input = "STOP";
			write(p1_fd,input.c_str(), input.length()+1);
			write(p2_fd,input.c_str(), input.length()+1);
			// display game info
			close(p1_fd);
			close(p2_fd);
			FD_CLR(p1_fd, &readfds);
                        FD_CLR(p2_fd, &readfds);
			connected = 0;
			maxc--;
			maxc--;
			break;
                    }
		    // this is the clt-d the user wants to stop
		    if(buffer[nread-1] == '\0') {
		      input.append(buffer, nread-1);
		      break;
		    }
		    else
		      input.append(buffer, nread);
		  }
		if(connected == 2){	
		  // need to listen for players answers
		  nread = read(p1_fd, buffer, buflen);
		  string p1a = buffer;
		  nread = read(p2_fd, buffer, buflen);
		  string p2a = buffer;
	
		  // decide who won

		
		  // add to the total score
			
	
		  // tell them who won
		  input = "Player 1 won";
                  write(p2_fd, input.c_str(), input.length()+1);
		  write(p1_fd, input.c_str(), input.length()+1);
		}
                }
            }
        }
    }
  return 0;
}

