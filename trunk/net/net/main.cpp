#include <iostream>
#include "Net.h"

using namespace std;

void main ()
{
	Net net (3, 3, 2);

	vector <float> dataIn;
	vector <float> *dataOut = NULL;

	dataIn.push_back (.1f);
	dataIn.push_back (0);
	dataIn.push_back (.1f);

	net.Work (&dataIn);

	int a = 0;
}