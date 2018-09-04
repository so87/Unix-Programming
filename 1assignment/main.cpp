#include <cmath>
#include <iostream>
#include "mytrig.h"
#include <fstream>

using namespace std;

double convertToRadians(double degrees)
{
	return degrees*M_PI/180.00;
}


int main()
{
	double answer;

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

	for(int i = 0; i < 19; i++)
	{
	  // Test sin	
	  answer = mysin(testData[i]);
	  cout << "My function: sin(" << testData[i] << ") = " << answer << endl;
	  cout << "Library    : sin(" << testData[i] << ") = " << sin(testData[i]) << endl;

	  // Test cos
	  answer = mycos(testData[i]);
	  cout << "My function: cos(" << testData[i] << ") = " << answer << endl;
	  cout << "Library    : cos(" << testData[i] << ") = " << cos(testData[i]) << endl;

	  // Test tan
	  answer = mytan(testData[i]);
	  cout << "My function: tan(" << testData[i] << ") = " << answer << endl;
	  cout << "Library    : tan(" << testData[i] << ") = " << tan(testData[i]) << endl;

	  cout << endl;
	}

	return 0;
}
