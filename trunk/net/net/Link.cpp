#include "Link.h"

Link::Link ()
{
	Weight = .5f;

	In = NULL;
	Out = NULL;
}

Link::~Link ()
{
	
}

bool Link::Transmit ()
{
	if (In && Out)
	{
		Out->Get (In->Excitement * Weight);

		return true;
	}

	return false;
}