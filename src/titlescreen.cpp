#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include "common.h"
#include "cmainmenu.h"

void TitleScreen()
{
    SDL_Event Event;
    CMainMenu *Menu = new CMainMenu;
    if (GlobalSoundEnabled)
    {
        if (!Mix_PlayingMusic())
            Mix_PlayMusic(Music,-1);
    }
    while (GameState == GSTitleScreen)
    {
        while(SDL_PollEvent(&Event))
        {
            if (Event.type == SDL_KEYDOWN)
			{
                switch (Event.key.keysym.sym)
                {
                    case SDLK_KP8:
                        Menu->PreviousItem();
                        break;
                    case SDLK_KP2:
                        Menu->NextItem();
                        break;
                    case SDLK_a:
                        if (GlobalSoundEnabled)
                            Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                        switch(Menu->GetSelection())
                        {
                            case 1:
                                GameState = GSGame;
                                break;
                            case 2:
                                GameState = GSOptions;
                                break;
                            case 3:
                                GameState = GSQuit;
                                break;
                        }
                        break;
                    case SDLK_b:
                        GameState = GSQuit;
					default:
						break;
                }
			}
        }
        Menu->Draw(Screen);
        SDL_Flip(Screen);
    }
    delete Menu;
}
