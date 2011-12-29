#pragma once

#include "Link.h"
#include <vector>

using namespace std;

class Neuron
{
public:

	bool Input;

	float Excitement;

	float Out;

	float d;

	vector <Link *> linksInput;
	vector <Link *> linksOutput;

	Neuron (bool Input = false);
	~Neuron ();

	void Get (float);
	bool Send ();

	static float Activation (float);
};