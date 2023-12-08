#include <pd_api.h>
#include "pd_helperfuncs.h"
#include "caudio.h"
#include "coptionmenu.h"
#include "common.h"

COptionMenu::COptionMenu(int Difficulty,bool JumpEnabled)
{
    Selection = 1;
    OptionDifficulty = Difficulty;
    OptionJump = JumpEnabled;

    IMGTitleScreen = loadImageAtPath("graphics/titlescreen.png");
    IMGVeryEasy1 = loadImageAtPath("graphics/veryeasy1.png");
    IMGVeryEasy2 = loadImageAtPath("graphics/veryeasy2.png");
    IMGEasy1 = loadImageAtPath("graphics/easy1.png");
    IMGEasy2 = loadImageAtPath("graphics/easy2.png");
    IMGHard1 = loadImageAtPath("graphics/hard1.png");
    IMGHard2 = loadImageAtPath("graphics/hard2.png");
    IMGVeryHard1 = loadImageAtPath("graphics/veryhard1.png");
    IMGVeryHard2 = loadImageAtPath("graphics/veryhard2.png");
    IMGJumpEnabled1 = loadImageAtPath("graphics/jumpenabled1.png");
    IMGJumpEnabled2 = loadImageAtPath("graphics/jumpenabled2.png");
    IMGJumpDisabled1 = loadImageAtPath("graphics/jumpdisabled1.png");
    IMGJumpDisabled2 = loadImageAtPath("graphics/jumpdisabled2.png");
}

// Destructor will free the surface images
COptionMenu::~COptionMenu()
{
    pd->graphics->freeBitmap(IMGTitleScreen);
    pd->graphics->freeBitmap(IMGVeryEasy1);
    pd->graphics->freeBitmap(IMGVeryEasy2);
    pd->graphics->freeBitmap(IMGEasy1);
    pd->graphics->freeBitmap(IMGEasy2);
    pd->graphics->freeBitmap(IMGHard1);
    pd->graphics->freeBitmap(IMGHard2);
    pd->graphics->freeBitmap(IMGVeryHard1);
    pd->graphics->freeBitmap(IMGVeryHard2);
    pd->graphics->freeBitmap(IMGJumpEnabled1);
    pd->graphics->freeBitmap(IMGJumpEnabled2);
    pd->graphics->freeBitmap(IMGJumpDisabled1);
    pd->graphics->freeBitmap(IMGJumpDisabled2);
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
    CAudio_PlaySound(Sounds[SND_SELECT],0);
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
    CAudio_PlaySound(Sounds[SND_SELECT],0);
}

void COptionMenu::NextItem()
{
    Selection++;
    if (Selection == 3)
        Selection = 1;
    CAudio_PlaySound(Sounds[SND_MENU],0);
}

void COptionMenu::PreviousItem()
{
    Selection--;
    if (Selection == 0)
        Selection = 2;
    CAudio_PlaySound(Sounds[SND_MENU],0);
}

// Draw the main menu
void COptionMenu::Draw(LCDBitmap *Surface)
{
    // draw the title screen background
    pd->graphics->drawBitmap(IMGTitleScreen, 0, 0, kBitmapUnflipped);


    if (Selection == 1)
    {
        if(OptionJump)
        {
            pd->graphics->drawBitmap(IMGJumpEnabled1, 40, 62, kBitmapUnflipped);
        }
        else
        {
			pd->graphics->drawBitmap(IMGJumpDisabled1, 40, 62, kBitmapUnflipped);
        }
    }
    else
    {
        if(OptionJump)
        {
			pd->graphics->drawBitmap(IMGJumpEnabled2, 40, 62, kBitmapUnflipped);
        }
        else
        {
            pd->graphics->drawBitmap(IMGJumpDisabled2, 40, 62, kBitmapUnflipped);
        }
    }

    if (Selection == 2)
    {
        switch(OptionDifficulty)
        {
            case VeryEasy:
                pd->graphics->drawBitmap(IMGVeryEasy1, 40, 119, kBitmapUnflipped);
                break;
            case Easy:
				pd->graphics->drawBitmap(IMGEasy1, 40, 119, kBitmapUnflipped);
				break;
            case Hard:
                pd->graphics->drawBitmap(IMGHard1, 40, 119, kBitmapUnflipped);
                break;
            case VeryHard:
                pd->graphics->drawBitmap(IMGVeryHard1, 40, 119, kBitmapUnflipped);
                break;
        }
    }
    else
    {
        switch(OptionDifficulty)
        {
   			case VeryEasy:
                pd->graphics->drawBitmap(IMGVeryEasy2, 40, 119, kBitmapUnflipped);
                break;
            case Easy:
				pd->graphics->drawBitmap(IMGEasy2, 40, 119, kBitmapUnflipped);
				break;
            case Hard:
                pd->graphics->drawBitmap(IMGHard2, 40, 119, kBitmapUnflipped);
                break;
            case VeryHard:
                pd->graphics->drawBitmap(IMGVeryHard2, 40, 119, kBitmapUnflipped);
                break;
        }
    }

}
