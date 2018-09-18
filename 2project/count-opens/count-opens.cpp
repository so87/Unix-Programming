// How many files can be opened: 256
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
	ifstream myFiles[1000];

	while(canOpen)
	{
	// open self
	for(int i = 0; i < 1000; i++){
	  myFiles[i].open("count-opens/test.txt");

	  // check if fail to open
	  if(!myFiles[i]){
	    break;
	  }
	  else if(i >= 999)
	    canOpen = false;
	  // increment if success
	  if (canOpen)
	    numFiles++;

	  // print out number of files so far	
	  cout << "Num files open: "<< numFiles << endl;		
	  }
	}
	for(int index = 0; index < 1000; index++)
	  myFiles[index].close();

	return 0;
}
