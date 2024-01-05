#include <iostream>
#include <vector>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
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
#include "credits.h"
#include "settings.h"

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
    Music = CAudio_LoadMusic("checkers");
}

void UnloadSounds()
{
    CAudio_UnLoadMusic(Music);
    int Teller;
    for (Teller=0;Teller<NrOfSounds;Teller++)
	if(Sounds[Teller])
		CAudio_UnLoadSound(Sounds[Teller]);
}

void resetGlobals()
{
	JumpHeuristicEnabled=false, SoundEnabled=true, MusicEnabled=true;
	GameState = GSTitleScreenInit;
	Difficulty = Hard;
	FrameTime = 0;
	Frames = 0; 
	CurrentMs = 0;
}

void setupGame()
{
	resetGlobals();
	CAudio_Init(false, 5.0f);
	Input = new CInput(pd, CINPUTDELAY);
	font = loadFontAtPath("fonts/font");
	LoadSounds();
	LoadSettings();
	CAudio_SetSoundEnabled(SoundEnabled);
	CAudio_SetMusicEnabled(MusicEnabled);
	CAudio_PlayMusic(Music, -1);
}

void TerminateGame()
{
	SaveSettings();
	UnloadSounds();
	CAudio_DeInit();
	delete Input;
	switch (GameState)
	{
		case GSGame:
			GameDeInit();
			break;
		case GSTitleScreen:
			TitleScreenDeInit();
			break;
		case GSOptions:
			OptionsDeInit();
			break;
		case GSCredits:
			CreditsDeInit();
			break;
		default:
			break;
	}
}

static int mainLoop(void* ud)
{
	pd->graphics->setDrawOffset(40,0);
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
		case GSCredits:
		case GSCreditsInit:
			Credits();
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
		playdate->system->setUpdateCallback(mainLoop, NULL);
		setupGame();
	}

	if (event == kEventTerminate)
	{
		TerminateGame();
	}
	return 0;
}

#ifdef __cplusplus
#ifndef SDL2API
}
#endif
#endif