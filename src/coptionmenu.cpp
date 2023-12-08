#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include "coptionmenu.h"
#include "common.h"

COptionMenu::COptionMenu(int Difficulty,bool JumpEnabled)
{
    Selection = 1;
    OptionDifficulty = Difficulty;
    OptionJump = JumpEnabled;
    SDL_Surface *Tmp;

    Tmp = IMG_Load("./Source/graphics/titlescreen.png");
    IMGTitleScreen = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/veryeasy1.png");
    IMGVeryEasy1 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/veryeasy2.png");
    IMGVeryEasy2 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/easy1.png");
    IMGEasy1 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/easy2.png");
    IMGEasy2 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/hard1.png");
    IMGHard1 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/hard2.png");
    IMGHard2 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/veryhard1.png");
    IMGVeryHard1 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/veryhard2.png");
    IMGVeryHard2 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/jumpenabled1.png");
    IMGJumpEnabled1 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/jumpenabled2.png");
    IMGJumpEnabled2 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/jumpdisabled1.png");
    IMGJumpDisabled1 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/jumpdisabled2.png");
    IMGJumpDisabled2 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

}

// Destructor will free the surface images
COptionMenu::~COptionMenu()
{
    SDL_FreeSurface(IMGTitleScreen);
    SDL_FreeSurface(IMGVeryEasy1);
    SDL_FreeSurface(IMGVeryEasy2);
    SDL_FreeSurface(IMGEasy1);
    SDL_FreeSurface(IMGEasy2);
    SDL_FreeSurface(IMGHard1);
    SDL_FreeSurface(IMGHard2);
    SDL_FreeSurface(IMGVeryHard1);
    SDL_FreeSurface(IMGVeryHard2);
    SDL_FreeSurface(IMGJumpEnabled1);
    SDL_FreeSurface(IMGJumpEnabled2);
    SDL_FreeSurface(IMGJumpDisabled1);
    SDL_FreeSurface(IMGJumpDisabled2);
}

void COptionMenu::NextOption()
{
    switch(Selection)
    {
        case 1:
            OptionJump = !OptionJump;
            break;
        case 2:
            OptionDifficulty++;
            if (OptionDifficulty > VeryHard)
                OptionDifficulty = VeryEasy;
            break;
    }
    if (GlobalSoundEnabled)
        Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
}

void COptionMenu::PreviousOption()
{
    switch(Selection)
    {
        case 1:
            OptionJump = !OptionJump;
            break;
        case 2:
            OptionDifficulty--;
            if (OptionDifficulty < VeryEasy)
                OptionDifficulty = VeryHard;
            break;
    }
    if (GlobalSoundEnabled)
        Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
}

void COptionMenu::NextItem()
{
    Selection++;
    if (Selection == 3)
        Selection = 1;
    if (GlobalSoundEnabled)
        Mix_PlayChannel(-1,Sounds[SND_MENU],0);
}

void COptionMenu::PreviousItem()
{
    Selection--;
    if (Selection == 0)
        Selection = 2;
    if (GlobalSoundEnabled)
        Mix_PlayChannel(-1,Sounds[SND_MENU],0);
}

// Draw the main menu
void COptionMenu::Draw(SDL_Surface *Surface)
{
    SDL_Rect Dest;

    // draw the title screen background
    SDL_BlitSurface(IMGTitleScreen,NULL,Surface,NULL);


    if (Selection == 1)
    {
        Dest.x = 40;
        Dest.y = 62;
        if(OptionJump)
        {
            Dest.w = IMGJumpEnabled1->w;
            Dest.h = IMGJumpEnabled1->h;
            SDL_BlitSurface(IMGJumpEnabled1,NULL,Surface,&Dest);
        }
        else
        {
            Dest.w = IMGJumpDisabled1->w;
            Dest.h = IMGJumpDisabled1->h;
            SDL_BlitSurface(IMGJumpDisabled1,NULL,Surface,&Dest);
        }
    }
    else
    {
        Dest.x = 40;
        Dest.y = 62;
        if(OptionJump)
        {
            Dest.w = IMGJumpEnabled2->w;
            Dest.h = IMGJumpEnabled2->h;
            SDL_BlitSurface(IMGJumpEnabled2,NULL,Surface,&Dest);
        }
        else
        {
            Dest.w = IMGJumpDisabled2->w;
            Dest.h = IMGJumpDisabled2->h;
            SDL_BlitSurface(IMGJumpDisabled2,NULL,Surface,&Dest);
        }
    }

    if (Selection == 2)
    {
        Dest.x = 40;
        Dest.y = 119;
        switch(OptionDifficulty)
        {
            case VeryEasy:
                Dest.w = IMGVeryEasy1->w;
                Dest.h = IMGVeryEasy1->h;
                SDL_BlitSurface(IMGVeryEasy1,NULL,Surface,&Dest);
                break;
            case Easy:
                Dest.w = IMGEasy1->w;
                Dest.h = IMGEasy1->h;
                SDL_BlitSurface(IMGEasy1,NULL,Surface,&Dest);
                break;
            case Hard:
                Dest.w = IMGHard1->w;
                Dest.h = IMGHard1->h;
                SDL_BlitSurface(IMGHard1,NULL,Surface,&Dest);
                break;
            case VeryHard:
                Dest.w = IMGVeryHard1->w;
                Dest.h = IMGVeryHard1->h;
                SDL_BlitSurface(IMGVeryHard1,NULL,Surface,&Dest);
                break;

        }

    }
    else
    {
        Dest.x = 40;
        Dest.y = 119;
        switch(OptionDifficulty)
        {
            case VeryEasy:
                Dest.w = IMGVeryEasy2->w;
                Dest.h = IMGVeryEasy2->h;
                SDL_BlitSurface(IMGVeryEasy2,NULL,Surface,&Dest);
                break;
            case Easy:
                Dest.w = IMGEasy2->w;
                Dest.h = IMGEasy2->h;
                SDL_BlitSurface(IMGEasy2,NULL,Surface,&Dest);
                break;
            case Hard:
                Dest.w = IMGHard2->w;
                Dest.h = IMGHard2->h;
                SDL_BlitSurface(IMGHard2,NULL,Surface,&Dest);
                break;
            case VeryHard:
                Dest.w = IMGVeryHard2->w;
                Dest.h = IMGVeryHard2->h;
                SDL_BlitSurface(IMGVeryHard2,NULL,Surface,&Dest);
                break;

        }
    }

}
