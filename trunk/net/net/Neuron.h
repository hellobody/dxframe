#pragma once

#include "Link.h"
#include <vector>

using namespace std;

class Neuron
{
public:

	float Excitement;

	vector <Link *> linksInput;
	vector <Link *> linksOutput;

	Neuron ();
	~Neuron ();

	void Get (float);
	bool Send ();
};