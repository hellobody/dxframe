#include "Neuron.h"

Neuron::Neuron ()
{
	Excitement = 0.f;
}

Neuron::~Neuron ()
{

}

void Neuron::Get (float subSignal)
{
	Excitement += subSignal;
}

bool Neuron::Send ()
{
	forup ((int) linksOutput.size ())
	{
		if (linksOutput [i])
		{
			linksOutput [i]->Transmit ();
		}
		else return false;
	}

	Excitement = 0;

	return true;
}