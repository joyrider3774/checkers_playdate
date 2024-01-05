#include <pd_api.h>
#include "options.h"
#include "common.h"
#include "coptionmenu.h"
#include "settings.h"

COptionMenu *OptionsMenu;

void ApplyOptions()
{
  	Difficulty = OptionsMenu->GetOptionDifficulty();
   	JumpHeuristicEnabled = OptionsMenu->GetOptionJump();

	if(SoundEnabled != OptionsMenu->GetOptionSound())
	{
		SoundEnabled = OptionsMenu->GetOptionSound();
		CAudio_SetSoundEnabled(SoundEnabled);
	}

	if(MusicEnabled != OptionsMenu->GetOptionMusic())
	{
		MusicEnabled = OptionsMenu->GetOptionMusic();
		CAudio_SetMusicEnabled(MusicEnabled);
		if(MusicEnabled)
			CAudio_PlayMusic(Music, -1);
	}

	SaveSettings();
}

void OptionsDeInit()
{
	delete OptionsMenu;
}

bool OptionsInit()
{
	OptionsMenu = new COptionMenu(Difficulty,JumpHeuristicEnabled, MusicEnabled, SoundEnabled);
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
			ApplyOptions();
		}

		if (Input->KeyboardPushed[SDLK_a] || Input->KeyboardPushed[SDLK_RIGHT])
		{
			OptionsMenu->NextOption();
			ApplyOptions();
		}

		if (Input->KeyboardPushed[SDLK_b])
		{
			GameState = GSTitleScreenInit;
		}

		OptionsMenu->Draw(NULL);
    }
	
	if((GameState != GSOptions) && (GameState != GSOptionsInit))
	{
    	OptionsDeInit();
	}
}