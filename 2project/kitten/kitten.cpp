#include <cmath>
#include <iostream>
#include <fstream>
#include <getopt.h>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
	int flags, opt;
	int sflag,nflag,eflag;
	sflag = 0;
	nflag = 0;
        eflag = 0;
	flags = 0;
	
	// keep going while there are arguments
	while ((opt = getopt(argc, argv, "Ens:")) != -1) {
	  switch (opt) {
	  // display $ at end of each line
	  case 'E':
		eflag = 1;
		flags++;
		break;
	  // number all output lines
	  case 'n':
		nflag = 1;
		flags++;
		break;
	  // supresses repeated empty output lines
	  case 's':
		sflag = 1;
		flags++;
		break;
	  // incorrect argument
	  default:
		fprintf(stderr, "Usage: %s [-E] [-n] [-s] filename\n", argv[0]);
		return 1;
	  }
	}

	// make sure the file exists
	if ((flags)>argc) {
	  fprintf(stderr, "Expected argument after options\n");
	  return 1;
	}
	
	// open it
	ifstream file;
	file.open(argv[argc-1]);
	if(!file.is_open()){
	  cout << "Error: that file doesn't exist" << endl;
	  return 1;
	}

	// for each line do E,n,s if they are marked
	string line;
	int numLines = 1;
	int blankLines = 0;
	int skip = 0;
	while (getline(file,line))
	{
	  // supress blank lines
	  if(sflag){
	    if(line.empty())
	      blankLines++;
	    else{
	      blankLines=0;
	      skip = 0;
	    }
	    //check if the rest of procedure needs to be run
	    if (blankLines > 1)
	      skip = 1;
	  }
	  if(!skip){
	    // number line
	    if(nflag){
	     cout << "     " << numLines << " "; 
	     numLines++;
	    }
	    // output regular line
	    cout << line;

	    // end line with $
	    if(eflag){
             cout << "$" << endl; 
	    }
	  }
	}
	file.close();

	return 0;
}

