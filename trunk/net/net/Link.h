#pragma once

#include <vld.h>

#define forup(x) for (int i=0; i<x; i++)
#define forupj(x) for (int j=0; j<x; j++)
#define forupk(x) for (int k=0; k<x; k++)

#include <stdio.h>
class Neuron;

class Link 
{
public:

	float Weight;

	Neuron *In;
	Neuron *Out;

	Link ();
	~Link ();
};