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
    int s_sfd, c_sfd;
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
                if(fd == s_sfd) {
                    c_len = sizeof(c_addr);
                    c_sfd = accept(s_sfd, &c_addr, &c_len);
                    FD_SET(c_sfd, &readfds);
                    cout << "client using fd " << c_sfd << endl;
   		    maxc++;
                }
                else {
		  // flush the input and start over
		  input.clear();
		  while(true) {
                    nread = read(fd, buffer, buflen);
                    if(nread == 0) {
                        close(fd);
			// this seems like magic
                        FD_CLR(fd, &readfds);
                        cout << "client releasing fd " << fd << endl;
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
		  // finally convert text to upper case
		  input = buffer;
                  write(fd, input.c_str(), input.length()+1);
                }
            }
        }
    }
  return 0;
}

