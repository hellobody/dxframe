#pragma once

#include "Link.h"
#include <vector>

using namespace std;

class Neuron
{
public:

	vector <Link *> links; 

	Neuron ();
	~Neuron ();

	void Get ();
	void Send ();
};