#include <cmath>
#include "mytrig.h"

using namespace std;

double mytan(double angle)
{
		double sin = mysin(angle);
		double cos = mycos(angle);
		return sin/cos;
}
