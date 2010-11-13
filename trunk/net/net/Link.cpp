#include "Link.h"
#include "Neuron.h"

Link::Link ()
{
	Weight = 1.f;

	In = NULL;
	Out = NULL;
}

Link::~Link ()
{
	
}

bool Link::Transmit (float Excitement)
{
	if (Out)
	{
		Out->Get (Excitement * Weight);

		return true;
	}

	return false;
}