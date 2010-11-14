#include <iostream>
#include "Net.h"

using namespace std;

void main ()
{
	Net net (2, 2, 5);

	vector <float> problem0;
	vector <float> problem1;
	vector <float> problem2;
	vector <float> solution0;
	vector <float> solution1;
	vector <float> solution2;
	vector <float> work;

	/////////////////////////////////////////////////
	problem0.push_back (1);
	problem0.push_back (0);
	solution0.push_back (0);
	solution0.push_back (1);
	/////////////////////////////////////////////////

	net.Train (&problem0, &solution0);

	net.Train (&problem0, &solution0);

	net.Train (&problem0, &solution0);

	net.Train (&problem0, &solution0);

	net.Train (&problem0, &solution0);

	net.Train (&problem0, &solution0);

	net.Train (&problem0, &solution0);

	work.push_back (0);
	work.push_back (1);

	net.Work (&work);

	int a = 0;
}