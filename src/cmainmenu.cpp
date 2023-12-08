#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include "cmainmenu.h"
#include "common.h"


// constructor of main menu will Load the graphics and set the current selection to 1 (io newgame)
CMainMenu::CMainMenu()
{
    Selection = 1;
    SDL_Surface *Tmp;

    Tmp = IMG_Load("./Source/graphics/titlescreen.png");
    IMGTitleScreen = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/play1.png");
    IMGPlay1 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/play2.png");
    IMGPlay2 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/options1.png");
    IMGOptions1 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/options2.png");
    IMGOptions2 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/quit1.png");
    IMGQuit1 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/quit2.png");
    IMGQuit2 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

}

// Destructor will free the surface images
CMainMenu::~CMainMenu()
{
    SDL_FreeSurface(IMGPlay1);
    SDL_FreeSurface(IMGPlay2);
    SDL_FreeSurface(IMGQuit1);
    SDL_FreeSurface(IMGQuit2);
    SDL_FreeSurface(IMGOptions1);
    SDL_FreeSurface(IMGOptions2);
    SDL_FreeSurface(IMGTitleScreen);
}

// Increase the selection if it goes to far set i to the first selection
void CMainMenu::NextItem()
{
    Selection++;
    if (Selection == 4)
        Selection = 1;
    if (GlobalSoundEnabled)
        Mix_PlayChannel(-1,Sounds[SND_MENU],0);
}

// decrease the selection if it goes to low set it to the last selection
void CMainMenu::PreviousItem()
{
    Selection--;
    if (Selection == 0)
        Selection = 3;
    if (GlobalSoundEnabled)
        Mix_PlayChannel(-1,Sounds[SND_MENU],0);
}

// Draw the main menu
void CMainMenu::Draw(SDL_Surface *Surface)
{
    SDL_Rect Dest;

    // draw the title screen background
    SDL_BlitSurface(IMGTitleScreen,NULL,Surface,NULL);


    if (Selection == 1)
    {
        Dest.x = 100;
        Dest.y = 62;
        Dest.w = IMGPlay1->w;
        Dest.h = IMGPlay1->h;
        SDL_BlitSurface(IMGPlay1,NULL,Surface,&Dest);
    }
    else
    {
        Dest.x = 100;
        Dest.y = 62;
        Dest.w = IMGPlay2->w;
        Dest.h = IMGPlay2->h;
        SDL_BlitSurface(IMGPlay2,NULL,Surface,&Dest);
    }

    if (Selection == 2)
    {
        Dest.x = 100;
        Dest.y = 107;
        Dest.w = IMGOptions1->w;
        Dest.h = IMGOptions1->h;
        SDL_BlitSurface(IMGOptions1,NULL,Surface,&Dest);
    }
    else
    {
        Dest.x = 100;
        Dest.y = 107;
        Dest.w = IMGOptions2->w;
        Dest.h = IMGOptions2->h;
        SDL_BlitSurface(IMGOptions2,NULL,Surface,&Dest);
    }


    if (Selection == 3) // = quit draw purple
    {
        Dest.x = 100;
        Dest.y = 150;
        Dest.w = IMGQuit1->w;
        Dest.h = IMGQuit1->h;
        SDL_BlitSurface(IMGQuit1,NULL,Surface,&Dest);
    }
    else // draw blue
    {
        Dest.x = 100;
        Dest.y = 150;
        Dest.w = IMGQuit2->w;
        Dest.h = IMGQuit2->h;
        SDL_BlitSurface(IMGQuit2,NULL,Surface,&Dest);
    }

}
