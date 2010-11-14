#include "Neuron.h"
#include <complex>

static const float a = 1.f;

float Neuron::Activation (float Y)
{
	float X = 1.f / (1.f + exp (-a * Y));
	return X;
}

Neuron::Neuron (bool Input)
{
	this->Input = Input;

	Excitement = 0.f;

	Out = 0.f;

	d = 0.f;
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
			Out = Input ? Excitement : Activation (Excitement);

			linksOutput [i]->Transmit (Out);
		}
		else return false;
	}

	Excitement = 0;

	return true;
}