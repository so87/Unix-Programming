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
      // for the size of the files/folders
      struct stat buf;
      char* completePath = (char*)malloc(snprintf(NULL, 0, "%s/%s", dirname.c_str(), ep->d_name) + 1);  
      sprintf(completePath, "%s/%s", dirname.c_str(), ep->d_name);
      stat(completePath, &buf);
    
      // fillout permissions 
      if (ep->d_type == DT_DIR)
      {
        permissions[0]='d';
      }
      permissions[1] = (buf.st_mode && S_IRUSR) ? 'r':'-';
      permissions[2] = (buf.st_mode && S_IWUSR) ? 'w':'-';
      permissions[3] = (buf.st_mode && S_IXUSR) ? 'x':'-';
      permissions[4] = (buf.st_mode && S_IRGRP) ? 'r':'-';
      permissions[5] = (buf.st_mode && S_IWGRP) ? 'w':'-';
      permissions[6] = (buf.st_mode && S_IXGRP) ? 'x':'-';
      permissions[7] = (buf.st_mode && S_IROTH) ? 'r':'-';
      permissions[8] = (buf.st_mode && S_IWOTH) ? 'w':'-';
      permissions[9] = (buf.st_mode && S_IXOTH) ? 'x':'-';
      permissions[10] = '\0';

      // get file user permissions


      // date time Month Day hr:min
      char date[256];
      struct tm time;
      struct stat sbuf;
      localtime_r(&sbuf.st_mtime, &time);
      strftime(date, sizeof(date), "%F %T", &time); 


      // permissions and date not working
      // print out each line
      printf("%s %zd %s %s \n", permissions, buf.st_size, date, ep->d_name);
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

