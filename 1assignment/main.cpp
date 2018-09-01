#include <cmath>
#include <iostream>
#include "mytrig.h"
#include <fstream>

using namespace std;

int main()
{
	double angle, answer;
	angle = 90;

	// Read the test data from the file and store it
	ofstream input;
	double testData[30];
	input.open("test.dat");
	int i = 0;
	while(!input.eof())
	{
	 cout << testData[i];
	 i++; 
	}	
	input.close();

	// Test sin
	answer = mysin(angle);
	cout << "sin(" << angle << ") = " << answer << " radians" << endl;

	// Test cos
	

	// Test tan


	return 0;
}
