#include "Neuron.h"

class Net 
{
public:

	vector <float> DataOut;

	vector <Neuron *> inputLayer;
	vector <Neuron *> outputLayer;

	Net (int sizeIn = 3, int sizeOut = 3, int numHideLayers = 2);
	~Net ();

	bool Train (vector <float> *DataIn, vector <float> *DataOutExpected);

	vector <float> * const Work (vector <float> *DataIn);
};