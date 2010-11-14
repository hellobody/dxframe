#include <iostream>
#include "Net.h"

using namespace std;

void main ()
{
	Net net (3, 3, 2);

	vector <float> dataIn;
	vector <float> dataOutExpect;

	dataIn.push_back (1);
	dataIn.push_back (0);
	dataIn.push_back (0);

	dataOutExpect.push_back (0);
	dataOutExpect.push_back (1);
	dataOutExpect.push_back (1);

	net.Train (&dataIn, &dataOutExpect);

	net.Work (&dataIn);

	int a = 0;
}