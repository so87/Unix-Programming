#include <iostream>
#include <fstream>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/dir.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <unistd.h>
#include <iomanip>


using namespace std;

int main(int argc, char * argv[])
{
  string dirname;

  if(argc == 1)
  {
   // scan the current directory
    dirname = ".";
  }
  else if(argc == 2)
  {
   // scan that directory
    dirname = argv[1];
  }  
  else
  {
	  cout << "Sorry this program only takes 0 or 1 arguments" << endl;
    cout << "Usage: ./lsl dir or ./lsl" << endl;
    return 1;
  }

  // scan the directory
  DIR *dp;
  struct dirent *ep;
  char permissions[11];

  // open the dir according to name. must concert string to char
  dp = opendir(dirname.c_str());
  
  // make sure it is there
  if (dp != NULL)
  {
    while (ep = readdir (dp))
    {
      // structs that hold our data
      struct stat buf;
      struct passwd perms;

      // figure out the block size
      char* completePath = (char*)malloc(snprintf(NULL, 0, "%s/%s", dirname.c_str(), ep->d_name) + 1);  
      sprintf(completePath, "%s/%s", dirname.c_str(), ep->d_name);
      stat(completePath, &buf);
    
      // get file user permissions 
      cout << geteuid() << endl;

      // date time Month Day hr:min
      char date[256];
      strcpy(date, ctime(&buf.st_mtime));
      memmove(date, date+4, strlen(date+4)+4);
      date[12] = '\0';

      // print out permissions, owner, group, id, size, date, and file name
      printf("%-10s %-7zd %-13s %-20s \n", perms.pw_name, buf.st_size, date, ep->d_name);
      free(completePath);
    }
    // close it out
    (void) closedir (dp);
  }
  else
  {
    cout << "error can't open directory" << endl;
    return 1;
  }

  return 0;
}

