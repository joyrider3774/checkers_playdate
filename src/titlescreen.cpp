#include <pd_api.h>
#include "pd_helperfuncs.h"
#include "caudio.h"
#include "common.h"
#include "cmainmenu.h"

CMainMenu *Menu;

void TitleScreenDeInit()
{
	pd->graphics->setDrawOffset(40, 0);
	delete Menu;
}

bool TitleScreenInit()
{
	Menu = new CMainMenu();
	pd->graphics->setBackgroundColor(kColorWhite);
	return true;
}

void TitleScreen()
{
    if (GameState == GSTitleScreenInit)
	{
		if (TitleScreenInit())
			GameState = GSTitleScreen;
	}
    
	if (GameState == GSTitleScreen)
	{
		if (Input->KeyboardPushed[SDLK_UP])
        	Menu->PreviousItem();
        
		if (Input->KeyboardPushed[SDLK_DOWN])
			Menu->NextItem();
		
		if (Input->KeyboardPushed[SDLK_a])
		{
        	CAudio_PlaySound(Sounds[SND_SELECT],0);
            switch(Menu->GetSelection())
            {
            	case 1:
                	GameState = GSGameInit;
                    break;
                case 2:
                    GameState = GSOptionsInit;
                    break;
                case 3:
                    GameState = GSCreditsInit;
                	break;
				default:
					break;
            }
		}
		pd->graphics->setDrawOffset(0, 0);
		pd->graphics->clear(kColorWhite);
        Menu->Draw(NULL);
    }

	if ((GameState != GSTitleScreen) && (GameState != GSTitleScreenInit))
    	TitleScreenDeInit();
}
