#include <iostream>
#include "mymath.h"
using namespace std;
int main()
{
     int a_int, b_int, c_int;
     cout << "Enter first number: ";
     cin  >> a_int;
     cout << "Enter second number: ";
     cin  >> b_int;
     c_int = sumtwo(a_int, b_int);
     cout << "Their sum is " << c_int << endl;
     return 0;
}