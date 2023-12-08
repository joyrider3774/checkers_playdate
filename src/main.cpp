#include <iostream>
#include <vector>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_Main.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "common.h"
#include "cbord.h"
#include "game.h"
#include "options.h"
#include "titlescreen.h"

using namespace std;


void LoadSounds()
{
    if(GlobalSoundEnabled)
    {
        Sounds[SND_START] = Mix_LoadWAV("./Source/sound/start.wav");
		Sounds[SND_SELECT] = Mix_LoadWAV("./Source/sound/select.wav");
		Sounds[SND_TAKEMOVE] = Mix_LoadWAV("./Source/sound/good.wav");
		Sounds[SND_WRONG] = Mix_LoadWAV("./Source/sound/wrong.wav");
		Sounds[SND_WINNER] = Mix_LoadWAV("./Source/sound/winner.wav");
		Sounds[SND_LOSER] = Mix_LoadWAV("./Source/sound/loser.wav");
		Sounds[SND_MENU] = Mix_LoadWAV("./Source/sound/menu.wav");
		Sounds[SND_MOVE] = Mix_LoadWAV("./Source/sound/drop.wav");
		Sounds[SND_DRAW] = Mix_LoadWAV("./Source/sound/draw.wav");
        Music = Mix_LoadMUS("./Source/music/checkers.ogg");
    }
}

void UnloadSounds()
{
    if (GlobalSoundEnabled)
    {
        if (!Mix_PlayingMusic())
            Mix_HaltMusic();
        Mix_FreeMusic(Music);
        int Teller;
        for (Teller=0;Teller<NrOfSounds;Teller++)
		if(Sounds[Teller])
			Mix_FreeChunk(Sounds[Teller]);
    }

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
		fprintf(SettingsFile,"Volume=%d\n",Volume);
        fprintf(SettingsFile,"Difficulty=%d\n",Difficulty);
        if (JumpHeuristicEnabled)
            fprintf(SettingsFile,"JumpHeuristic=1\n");
        else
            fprintf(SettingsFile,"JumpHeuristic=0\n");
		fclose(SettingsFile);
//		sync();
 	}
}

int main(int argc, char **argv)
{
	if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) == 0)
	{
		printf("SDL Succesfully initialized\n");
        Screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT,16, SDL_SWSURFACE );
		if(Screen)
		{
		    printf("Succesfully Set %dx%dx16\n",WINDOW_WIDTH,WINDOW_HEIGHT);

			SDL_ShowCursor(SDL_DISABLE);
			{
				if (Mix_OpenAudio(22050,AUDIO_S16,MIX_DEFAULT_CHANNELS,1024) < 0)
				{
					GlobalSoundEnabled = false;
					printf("Failed to initialise sound!\n");
					printf("%s\n",Mix_GetError());
				}
				else
				{
					printf("Audio Succesfully initialised!\n");
				}
				if (TTF_Init() == 0)
				{
					printf("Succesfully initialized TTF\n");
					font = TTF_OpenFont("./Source/fonts/font.ttf",18);
					if (font)
					{
						printf("Succesfully Loaded fonts\n");
						TTF_SetFontStyle(font,TTF_STYLE_NORMAL);
						LoadSounds();
						LoadSettings();
						while (GameState != GSQuit)
						{
							switch(GameState)
							{
								case GSGame :
									Game();
									break;
								case GSTitleScreen:
									TitleScreen();
									break;
								case GSOptions:
									Options();
									break;
								default:
									break;
							}
						}
						SaveSettings();
						UnloadSounds();
						Mix_CloseAudio();
						TTF_CloseFont(font);
						font=NULL;
					}
					else
					{
						printf("Failed to Load fonts\n");
					}
					TTF_Quit();
				}
				else
				{
					printf("Failed to initialize TTF\n");
				}
			}
			SDL_FreeSurface(Screen);
            Screen=NULL;
		}
		else
		{
			printf("Failed to Set Videomode %dx%dx16\n",WINDOW_WIDTH, WINDOW_HEIGHT);
		}
		SDL_Quit();
	}
	else
	{
		printf("Couldn't initialise SDL!\n");
	}

	return 0;

}
