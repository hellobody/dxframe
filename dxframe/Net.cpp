#include "stdafx.h"

#include "Net.h"

static float n = 1.f;

Net::Net (int sizeIn, int sizeOut, int numHideLayers)
{
	vector <Neuron *> prewLayer;

	forup (sizeIn)
	{
		inputLayer.push_back (new Neuron (true));
	}

	forup ((int) inputLayer.size ()) prewLayer.push_back (inputLayer [i]);

	forup (numHideLayers)
	{
		int sizeOfNewHideLayer = sizeIn + ((sizeOut - sizeIn) / (numHideLayers + 1)) * (i + 1);

		vector <Neuron *> *hideLayer = new vector <Neuron *>;

		forupj (sizeOfNewHideLayer)
		{
			hideLayer->push_back (new Neuron);

			forupk ((int) prewLayer.size ())
			{
				Link *tpLink = new Link;

				tpLink->In = prewLayer [k];
				tpLink->Out = (*hideLayer) [(int) hideLayer->size ()-1];

				prewLayer [k]->linksOutput.push_back (tpLink);
				(*hideLayer) [(int) hideLayer->size ()-1]->linksInput.push_back (tpLink);
			}
		}

		pHideLayers.push_back (hideLayer);

		prewLayer.clear ();
		forupj ((int) hideLayer->size ()) prewLayer.push_back ((*hideLayer) [j]);
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
	RELEASE_VECTOR (inputLayer);
	RELEASE_VECTOR (outputLayer);

	forup ((int) pHideLayers.size ()) {

		RELEASE_VECTOR (*(pHideLayers [i]));
	}
	RELEASE_VECTOR (pHideLayers);
}

void Net::Train (vector <float> *DataIn, vector <float> *DataOutExpect)
{
	vector <float> Out;
	
	Out = Work (DataIn);

	forup ((int) Out.size ())
	{
		outputLayer [i]->d = Out [i] * (1.f - Out [i]) * ((*DataOutExpect) [i] - Out [i]);
	}

	forup ((int) outputLayer.size ())
	{
		forupj ((int) outputLayer [i]->linksInput.size ())
		{
			outputLayer [i]->linksInput [j]->Weight += n * outputLayer [i]->d * outputLayer [i]->linksInput [j]->In->Out;
		}
	}

	forup ((int) pHideLayers.size ())
	{
		forupj ((int) (*pHideLayers [(int) pHideLayers.size ()-1 - i]).size ())
		{
			float E = 0;

			forupk ((int) (*pHideLayers [(int) pHideLayers.size ()-1 - i]) [j]->linksOutput.size ())
			{
				E += (*pHideLayers [(int) pHideLayers.size ()-1 - i]) [j]->linksOutput [k]->Out->d * (*pHideLayers [(int) pHideLayers.size ()-1 - i]) [j]->linksOutput [k]->Weight;
			}

			float Out = (*pHideLayers [(int) pHideLayers.size ()-1 - i]) [j]->Out;
			(*pHideLayers [(int) pHideLayers.size ()-1 - i]) [j]->d = Out * (1.f - Out) * E;


			forupk ((int) (*pHideLayers [(int) pHideLayers.size ()-1 - i]) [j]->linksInput.size ())
			{
				(*pHideLayers [(int) pHideLayers.size ()-1 - i]) [j]->linksInput [k]->Weight += n * (*pHideLayers [(int) pHideLayers.size ()-1 - i]) [j]->d * (*pHideLayers [(int) pHideLayers.size ()-1 - i]) [j]->linksInput [k]->In->Out;
			}
		}
	}
}

vector <float> const& Net::Work (vector <float> *DataIn)
{
	DataOut.clear ();

	forup ((int) inputLayer.size ())
	{
		inputLayer [i]->Excitement = (*DataIn) [i];
	}

	forup ((int) inputLayer.size ())
	{
		inputLayer [i]->Send ();
	}

	forup ((int) pHideLayers.size ())
	{
		forupj ((int) pHideLayers [i]->size ())
		{
			(*pHideLayers [i]) [j]->Send ();
		}
	}

	forup ((int) outputLayer.size ())
	{
		DataOut.push_back (Neuron::Activation (outputLayer [i]->Excitement));
	}

	return DataOut;
}