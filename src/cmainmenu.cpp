#include <pd_api.h>
#include "pd_helperfuncs.h"
#include "caudio.h"
#include "cmainmenu.h"
#include "common.h"


// constructor of main menu will Load the graphics and set the current selection to 1 (io newgame)
CMainMenu::CMainMenu()
{
    Selection = 1;
    IMGTitleScreen = loadImageAtPath("graphics/titlescreen.png");
    IMGPlay1 = loadImageAtPath("graphics/play1.png");
    IMGPlay2 = loadImageAtPath("graphics/play2.png");
    IMGOptions1 = loadImageAtPath("graphics/options1.png");
    IMGOptions2 = loadImageAtPath("graphics/options2.png");
    IMGCredits1 = loadImageAtPath("graphics/credits1.png");
    IMGCredits2 = loadImageAtPath("graphics/credits2.png");
}

// Destructor will free the surface images
CMainMenu::~CMainMenu()
{
    pd->graphics->freeBitmap(IMGPlay1);
    pd->graphics->freeBitmap(IMGPlay2);
    pd->graphics->freeBitmap(IMGCredits1);
    pd->graphics->freeBitmap(IMGCredits2);
    pd->graphics->freeBitmap(IMGOptions1);
    pd->graphics->freeBitmap(IMGOptions2);
    pd->graphics->freeBitmap(IMGTitleScreen);
}

// Increase the selection if it goes to far set i to the first selection
void CMainMenu::NextItem()
{
    Selection++;
    if (Selection == 4)
        Selection = 1;
    CAudio_PlaySound(Sounds[SND_MENU],0);
}

// decrease the selection if it goes to low set it to the last selection
void CMainMenu::PreviousItem()
{
    Selection--;
    if (Selection == 0)
        Selection = 3;
    CAudio_PlaySound(Sounds[SND_MENU],0);
}

// Draw the main menu
void CMainMenu::Draw(LCDBitmap *Surface)
{
    // draw the title screen background
    pd->graphics->drawBitmap(IMGTitleScreen, 0, 0, kBitmapUnflipped);


    if (Selection == 1)
    {
        pd->graphics->drawBitmap(IMGPlay1, 140, 62, kBitmapUnflipped);
    }
    else
    {
        pd->graphics->drawBitmap(IMGPlay2, 140, 62, kBitmapUnflipped);
    }

    if (Selection == 2)
    {
        pd->graphics->drawBitmap(IMGOptions1, 140, 107, kBitmapUnflipped);
	}
    else
    {
        pd->graphics->drawBitmap(IMGOptions2, 140, 107, kBitmapUnflipped);
    }


    if (Selection == 3) // = quit draw purple
    {
        pd->graphics->drawBitmap(IMGCredits1, 140, 150, kBitmapUnflipped);
    }
    else // draw blue
    {
        pd->graphics->drawBitmap(IMGCredits2, 140, 150, kBitmapUnflipped);
    }

}
