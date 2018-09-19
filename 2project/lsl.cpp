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
    int totalSize = 0;
    while (ep = readdir (dp))
    {
      // structs that hold our data
      struct stat buf;
      lstat(ep->d_name, &buf);
      string fileName = ep->d_name;
      // make sure to exclude . and .. from the command
      if((fileName != ".") && (fileName != "..")){ 
      // get size
      totalSize += (int)buf.st_size;

        // get dir
	if(buf.st_mode & S_IFDIR)
	  permissions[0] = 'd';
	else
	  permissions[0] = '-';
	// get owner perms
	if(buf.st_mode & S_IRUSR)
          permissions[1] = 'r';
	else
          permissions[1] = '-';
	if(buf.st_mode & S_IWUSR)
          permissions[2] = 'w';
	else
          permissions[2] = '-';
	if(buf.st_mode & S_IXUSR)
          permissions[3] = 'x';
        else
          permissions[3] = '-';
	// get group permissions
	if(buf.st_mode & S_IRGRP)
          permissions[4] = 'r';
        else
          permissions[4] = '-';
        if(buf.st_mode & S_IWGRP)
          permissions[5] = 'w';
        else
          permissions[5] = '-';
        if(buf.st_mode & S_IXGRP)
          permissions[6] = 'x';
        else
          permissions[6] = '-';

	// get other permissionss
	if(buf.st_mode & S_IROTH)
          permissions[7] = 'r';
        else
          permissions[7] = '-';
        if(buf.st_mode & S_IWOTH)
          permissions[8] = 'w';
        else
          permissions[8] = '-';
        if(buf.st_mode & S_IXOTH)
          permissions[9] = 'x';
        else
          permissions[9] = '-';

	// get file user permissions 
      string permID = getpwuid(buf.st_uid)->pw_name;
      string permGP = getgrgid(buf.st_gid)->gr_name;
      
      // date time Month Day hr:min
      char date[256];
      strcpy(date, ctime(&buf.st_mtime));
      memmove(date, date+4, strlen(date+4)+4);
      date[12] = '\0';

      // print out permissions, owner, group, id, size, date, and file name
      cout << setw(11) << left << permissions;
      cout << setw(2) << left << buf.st_nlink;
      cout << setw(5) << left << permID;
      cout << setw(5) << left << permGP;
      cout << setw(8) << left << buf.st_size;
      cout << setw(11) << left << date << " ";
      cout << left << setw(15) << fileName << endl;
      }
    }
    // close it out
    totalSize = (totalSize)/(1024);
    cout << "total " << totalSize << endl;
    (void) closedir (dp);
  }
  else
  {
    cout << "error can't open directory" << endl;
    return 1;
  }  
  // sort that output strig

  // print everything out now that it is organized


  return 0;
}

