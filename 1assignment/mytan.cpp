#include <cmath>
#include "mytrig.h"

using namespace std;

double mytan(double angle)
{
	if (abs(angle) <= 0.1)
		return angle;
	else{
		double sin = mysin(angle);
		double cos = mycos(angle);
		return sin/cos;
	}
}
