#include <SDL/SDL.h>
#include "options.h"
#include "common.h"
#include "coptionmenu.h"

void Options()
{
    SDL_Event Event;
    COptionMenu *Menu = new COptionMenu(Difficulty,JumpHeuristicEnabled);
    while (GameState == GSOptions)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_KEYDOWN)
            {
                switch(Event.key.keysym.sym)
                {
                    case SDLK_KP8:
                        Menu->PreviousItem();
                        break;
                    case SDLK_KP2:
                        Menu->NextItem();
                        break;
                    case SDLK_KP4:
                        Menu->PreviousOption();
                        break;
                    case SDLK_KP6:
                        Menu->NextOption();
                        break;
                    default:
                        GameState = GSTitleScreen;
                }
            }
        }
        Menu->Draw(Screen);
        SDL_Flip(Screen);
    }
    Difficulty = Menu->GetOptionDifficulty();
    JumpHeuristicEnabled = Menu->GetOptionJump();
    delete Menu;
}