// How many files can be opened: 
#include <cmath>
#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	// open files
	int canOpen = 1;
	int numFiles = 0;
	fstream file;
	while(canOpen)
	{
	// checking if fail
	file.open("count-opens.exe");
	canOpen = file.is_open();
	
	// increment if success
	if (canOpen)
	  numFiles++;

	// print out number of files so far	
	cout << "Num files open: "<< numFiles << endl;		
	}

	// lose all of those open files`
	void close();

	return 0;
}
