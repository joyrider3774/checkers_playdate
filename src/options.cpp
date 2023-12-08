#include <pd_api.h>
#include "options.h"
#include "common.h"
#include "coptionmenu.h"

COptionMenu *OptionsMenu;

bool OptionsInit()
{
	OptionsMenu = new COptionMenu(Difficulty,JumpHeuristicEnabled);
	return true;
}

void Options()
{  
	if (GameState == GSOptionsInit)
	{
		if (OptionsInit())
			GameState = GSOptions;
	}

    if (GameState == GSOptions)
    {
       	if (Input->KeyboardPushed[SDLK_UP])
		{
    		OptionsMenu->PreviousItem();
		}
        
		if (Input->KeyboardPushed[SDLK_DOWN])
		{
			OptionsMenu->NextItem();
		}
			
		if (Input->KeyboardPushed[SDLK_LEFT])
		{
			OptionsMenu->PreviousOption();
		}

		if (Input->KeyboardPushed[SDLK_a] || Input->KeyboardPushed[SDLK_RIGHT])
		{
			OptionsMenu->NextOption();
		}

		if (Input->KeyboardPushed[SDLK_b])
		{
			GameState = GSTitleScreenInit;
		}

		OptionsMenu->Draw(NULL);
    }
	
	if((GameState != GSOptions) && (GameState != GSOptionsInit))
	{
    	Difficulty = OptionsMenu->GetOptionDifficulty();
    	JumpHeuristicEnabled = OptionsMenu->GetOptionJump();
    	delete OptionsMenu;
	}
}