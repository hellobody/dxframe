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
		int sizeOfNewHideLayer = sizeIn + ((sizeOut - sizeIn) / (numHideLayers + 1)) * (i + 1);

		vector <Neuron *> hideLayer;

		forupj (sizeOfNewHideLayer)
		{
			hideLayer.push_back (new Neuron);

			forupk ((int) prewLayer.size ())
			{
				Link *tpLink = new Link;

				tpLink->In = prewLayer [k];
				tpLink->Out = hideLayer [(int) hideLayer.size ()-1];

				prewLayer [k]->linksOutput.push_back (tpLink);
				hideLayer [(int) hideLayer.size ()-1]->linksInput.push_back (tpLink);
			}
		}

		prewLayer.clear ();
		forupj ((int) hideLayer.size ()) prewLayer.push_back (hideLayer [j]);
	}

	forup (sizeOut)
	{
		outputLayer.push_back (new Neuron);

		forupj ((int) prewLayer.size ())
		{
			Link *tpLink = new Link;

			tpLink->In = prewLayer [j];
			tpLink->Out = outputLayer [(int) outputLayer.size ()-1];

			prewLayer [j]->linksOutput.push_back (tpLink);
			outputLayer [(int) outputLayer.size ()-1]->linksInput.push_back (tpLink);
		}
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