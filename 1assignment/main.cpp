#include <cmath>
#include <iostream>
#include "mytrig.h"
#include <fstream>

using namespace std;

double convertToRadians(double degrees)
{
	return degrees*3.141592/180;
}


int main()
{
	double angle, answer;
	angle = 90;

	// Read the test data from the file and store it
	ifstream input;
	double testData[30], temp;
	input.open("test.dat");

	// Make sure it can open
	if (!input) 
	{
	  cerr << "Can't open file" << endl;
		  exit(1);
	}

	// Read in the data
	int i = 0;
	while(input >> temp)
	{
	 testData[i] = convertToRadians(temp);
	 i++; 
	}	
	input.close();

	// Get the size of the array
	int size = sizeof(testData)/sizeof(*testData);	
	angle = convertToRadians(angle);

	for(int i = 0; i < size; i++)
	{
	  // Test sin	
	  answer = mysin(testData[i]);
	  cout << "sin(" << angle << ") = " << answer << endl;

	  // Test cos
	  answer = mycos(angle);
	  cout << "cos(" << angle << ") = " << answer << endl;

	  // Test tan
	  answer = mytan(angle);
	  cout << "tan(" << angle << ") = " << answer << endl;
	}

	return 0;
}
