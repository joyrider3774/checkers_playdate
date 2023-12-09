#include <pd_api.h>
#include "pd_helperfuncs.h"
#include "credits.h"
#include "caudio.h"
#include "common.h"

LCDBitmap *IMGCredits, *IMGTitleScreen;

bool CreditsInit()
{
	IMGCredits = loadImageAtPath("graphics/credits");
	IMGTitleScreen = loadImageAtPath("graphics/titlescreen");
	pd->display->setOffset(0, 0);
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
		
		pd->graphics->drawBitmap(IMGCredits, 0, 0, kBitmapUnflipped);
    }
	
	if ((GameState != GSCreditsInit) && (GameState != GSCredits))
	{
		pd->display->setOffset(40, 0);	
		pd->graphics->clear(kColorWhite);
		pd->graphics->drawBitmap(IMGTitleScreen, 0, 0, kBitmapUnflipped);
		pd->graphics->freeBitmap(IMGCredits);
		pd->graphics->freeBitmap(IMGTitleScreen);
	}
}