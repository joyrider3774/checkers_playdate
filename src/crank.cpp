#include "pd_helperfuncs.h"
#include "crank.h"

float moveThreshold = 0;
float currentThreshold = 0;

unsigned int crankUpdate()
{
	if (pd->system->isCrankDocked())
	{
		return CRANKDOCKED;
	}
	else
	{
		currentThreshold += pd->system->getCrankChange();
		if (currentThreshold >= moveThreshold)
		{
			currentThreshold = 0;
			return CRANKMOVERIGHT;
		}
		else
		{
			if (currentThreshold <= -moveThreshold)
			{
				currentThreshold = 0;
				return CRANKMOVELEFT;
			}
			else
			{
				return CRANKMOVENONE;
			}
		}
	}
}

void setCrankMoveThreshold(float degrees)
{
	moveThreshold = degrees;
}