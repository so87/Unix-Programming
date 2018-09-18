// How many files can be opened: 3200 including the 3
// Confirm by typing "ulimit -n"
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char * argv[])
{
	// open files
	int canOpen = 1;
	int numFiles = 0;
	fstream myFiles[4000];

	while(canOpen)
	{
	// open self
	for(int i = 0; i < 4000; i++){
	  myFiles[i].open("count-opens/test.txt");

	  // check if fail to open
	  if(!myFiles[i].is_open()){
	    cout << "Max files: " << numFiles << endl;
	    return 1;
	  }
	  else if(i >= 3999)
	    canOpen = false;
	  // increment if success
	  if (canOpen)
	    numFiles++;

	  // print out number of files so far	
	  cout << "Num files open: "<< numFiles << endl;		
	  }
	}
	for(int index = 0; index < 2000; index++)
	  myFiles[index].close();

	return 0;
}
