#include "mytrig.h"
#include <cmath>

using namespace std;

double mycos(double angle)
{
	if (abs(angle) <= 0.1)
		return angle;
	else
		return mysin(angle+3.1415926535/2.0);
}
