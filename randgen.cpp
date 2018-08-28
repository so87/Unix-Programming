// Print 10 random 32-bit integers
#include <fstream>
#include <iostream>

using namespace std;

int main(void)
{
	// Try type changes to short, unsigned short, char, etc
   int randnum;

   ifstream randstr("/dev/random");

   for(int count=0; count<10; count++) {
      randstr.read((char *)&randnum,sizeof(randnum));
      cout << count << ": " << randnum << endl;
   }
   randstr.close();
   return 0;
}