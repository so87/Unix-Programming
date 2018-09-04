#include <cmath>
#include "mytrig.h"
#include <iostream>

using namespace std;

static double utility(double y)
{ return 3.*y - 4.*y*y*y; }

double mysin(double angle)
{
	if (abs(angle) <= 0.1)
	{	
		cout << "WARNING ANGLE LESS THAN .1" << endl;
		return angle;
	}
		else
		return utility(mysin(angle/3.));
}
