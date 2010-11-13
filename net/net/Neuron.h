#pragma once

#include "Link.h"
#include <vector>

using namespace std;

class Neuron
{
public:

	vector <Link *> linksInput;
	vector <Link *> linksOutput;

	Neuron ();
	~Neuron ();

	void Get ();
	void Send ();
};