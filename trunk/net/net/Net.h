#include "Neuron.h"

class Net 
{
public:

	vector <float> DataOut;

	vector <Neuron *> inputLayer;
	vector <Neuron *> outputLayer;

	vector <vector <Neuron *> *> pHideLayers;

	Net (int sizeIn = 3, int sizeOut = 3, int numHideLayers = 2);
	~Net ();

	void Train (vector <float> *DataIn, vector <float> *DataOutExpect);

	vector <float> const& Work (vector <float> *DataIn);
};