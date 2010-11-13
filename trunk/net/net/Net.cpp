#include "Net.h"

Net::Net (int sizeIn, int sizeOut, int numHideLayers)
{
	vector <Neuron *> prewLayer;

	forup (sizeIn)
	{
		inputLayer.push_back (new Neuron);
	}

	forup ((int) inputLayer.size ()) prewLayer.push_back (inputLayer [i]);

	forup (numHideLayers)
	{
		
	}
}

Net::~Net ()
{

}

bool Net::Train (vector <float> *DataIn, vector <float> *DataOutExpected)
{
	return true;
}

vector <float> * const Net::Work (vector <float> *DataIn)
{
	return &DataOut;
}