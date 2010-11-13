#pragma once

#define forup(x) for (int i=0; i<x; i++)

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