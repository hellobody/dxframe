#include "Neuron.h"

class Net 
{
public:

	vector <Neuron *> inputLayer;
	vector <Neuron *> outputLayer;

	vector <vector <Neuron *> *> pHideLayers;

	Net (int sizeIn = 3, int sizeOut = 3, int numHideLayers = 2);
	~Net ();

	bool Train (vector <float> *DataIn, vector <float> *DataOutExpected);

	vector <float> * const Work (vector <float> *DataIn);
};