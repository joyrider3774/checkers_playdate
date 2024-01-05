#include <pd_api.h>
#include "pd_helperfuncs.h"
#include "credits.h"
#include "caudio.h"
#include "common.h"

LCDBitmap *IMGCredits, *IMGTitleScreen;

void CreditsDeInit()
{
	pd->graphics->clear(kColorWhite);
	pd->graphics->setDrawOffset(40, 0);	
	pd->graphics->drawBitmap(IMGTitleScreen, 0, 0, kBitmapUnflipped);
	pd->graphics->freeBitmap(IMGCredits);
	pd->graphics->freeBitmap(IMGTitleScreen);
}

bool CreditsInit()
{
	IMGCredits = loadImageAtPath("graphics/credits");
	IMGTitleScreen = loadImageAtPath("graphics/titlescreen");
	return true;
}

void Credits()
{  
	if (GameState == GSCreditsInit)
	{
		if(CreditsInit())
			GameState = GSCredits;
	}

	if (GameState == GSCredits)
	{	
		if (Input->KeyboardPushed[SDLK_a] || Input->KeyboardPushed[SDLK_b])
		{
			GameState = GSTitleScreenInit;
     	}
		pd->graphics->setDrawOffset(0, 0);
		pd->graphics->clear(kColorWhite);
		pd->graphics->drawBitmap(IMGCredits, 0, 0, kBitmapUnflipped);
    }
	
	if ((GameState != GSCreditsInit) && (GameState != GSCredits))
	{
		CreditsDeInit();
	}
}