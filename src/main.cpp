#include <iostream>
#include <vector>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#ifndef SDL2API
#include <pdcpp/pdnewlib.h>
#endif
#include <pd_api.h>
#include "pd_helperfuncs.h"
#include "caudio.h"
#include "common.h"
#include "cbord.h"
#include "game.h"
#include "options.h"
#include "titlescreen.h"


using namespace std;


void LoadSounds()
{
    Sounds[SND_START] = CAudio_LoadSound("start");
	Sounds[SND_SELECT] = CAudio_LoadSound("select");
	Sounds[SND_TAKEMOVE] = CAudio_LoadSound("good");
	Sounds[SND_WRONG] = CAudio_LoadSound("wrong");
	Sounds[SND_WINNER] = CAudio_LoadSound("winner");
	Sounds[SND_LOSER] = CAudio_LoadSound("loser");
	Sounds[SND_MENU] = CAudio_LoadSound("menu");
	Sounds[SND_MOVE] = CAudio_LoadSound("drop");
	Sounds[SND_DRAW] = CAudio_LoadSound("draw");
	Sounds[SND_MUSIC] = CAudio_LoadSound("checkers"); //so it loads into memory
    //Music = CAudio_LoadMusic("checkers");
}

void UnloadSounds()
{
    CAudio_UnLoadMusic(Music);
    int Teller;
    for (Teller=0;Teller<NrOfSounds;Teller++)
	if(Sounds[Teller])
		CAudio_UnLoadSound(Sounds[Teller]);
}

void LoadSettings()
{
 	FILE *SettingsFile;
 	int Value=0;
 	SettingsFile = fopen("./settings.dat","r");
 	if(SettingsFile)
 	{
        fscanf(SettingsFile,"Difficulty=%d\n",&Difficulty);
        fscanf(SettingsFile,"JumpHeuristic=%d\n",&Value);
        if (Value == 1)
            JumpHeuristicEnabled = true;
        else
            JumpHeuristicEnabled = false;
		fclose(SettingsFile);
 	}
 	else
 	{
        Difficulty = Hard;
        JumpHeuristicEnabled = false;
	}
}

// Save the settings
void SaveSettings()
{
 	FILE *SettingsFile;
 	SettingsFile = fopen("./settings.dat","w");
 	if(SettingsFile)
 	{
        fprintf(SettingsFile,"Difficulty=%d\n",Difficulty);
        if (JumpHeuristicEnabled)
            fprintf(SettingsFile,"JumpHeuristic=1\n");
        else
            fprintf(SettingsFile,"JumpHeuristic=0\n");
		fclose(SettingsFile);
//		sync();
 	}
}

void setupGame()
{
	CAudio_Init(false);
	Input = new CInput(pd, CINPUTDELAY);
	font = loadFontAtPath("fonts/font");
	LoadSounds();
	//CAudio_PlayMusic(Music, -1);
	CAudio_PlaySound(Sounds[SND_MUSIC], -1);
	//LoadSettings();
}

void destroyGame()
{
	//SaveSettings();
	UnloadSounds();
	CAudio_DeInit();
}

static int mainLoop(void* ud)
{
	Input->Update();
	switch (GameState)
	{
		case GSGame:
		case GSGameInit:
			Game();
			break;
		case GSTitleScreen:
		case GSTitleScreenInit:
			TitleScreen();
			break;
		case GSOptions:
		case GSOptionsInit:
			Options();
			break;
		default:
			break;
	}
	
	if (showFPS)
	{
		Frames++;
		if (pd->system->getCurrentTimeMilliseconds() - FrameTime >= 1000)
		{
			CurrentMs = (float)(1000.0f / Frames);
			Frames = 0;
			FrameTime += 1000;
		}

		char* TmpText;
		pd->system->formatString(&TmpText, "FPS: %.0f\n", 1000.0f / CurrentMs);
		pd->graphics->drawText(TmpText, strlen(TmpText), kASCIIEncoding, 0, 0);
		pd->system->realloc(TmpText, 0);

	}

	return 1;
}


#ifdef __cplusplus
#ifndef SDL2API
extern "C" {
#endif
#endif

#ifdef _WINDLL
__declspec(dllexport)
#endif

int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
	if ( event == kEventInit )
	{
		#ifndef SDL2API
		eventHandler_pdnewlib(playdate, event, arg);		
		#endif
		setPDPtr(playdate);
		playdate->display->setRefreshRate(FRAMERATE);
		playdate->display->setOffset(40,0);
		playdate->system->setUpdateCallback(mainLoop, NULL);
		setupGame();
	}

	if (event == kEventTerminate)
	{
		destroyGame();
	}
	return 0;
}

#ifdef __cplusplus
#ifndef SDL2API
}
#endif
#endif