#include <pd_api.h>
#include "pd_helperfuncs.h"
#include "caudio.h"
#include "coptionmenu.h"
#include "common.h"

COptionMenu::COptionMenu(int Difficulty,bool JumpEnabled, bool MusicEnabled, bool SoundEnabled)
{
    Selection = 1;
    OptionDifficulty = Difficulty;
    OptionJump = JumpEnabled;
	OptionSound = SoundEnabled;
	OptionMusic = MusicEnabled;

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

	IMGMusicEnabled1 = loadImageAtPath("graphics/musicenabled1.png");
	IMGMusicEnabled2 = loadImageAtPath("graphics/musicenabled2.png");
	IMGMusicDisabled1 = loadImageAtPath("graphics/musicdisabled1.png");
	IMGMusicDisabled2 = loadImageAtPath("graphics/musicdisabled2.png");
	IMGSoundEnabled1 = loadImageAtPath("graphics/soundenabled1.png");
	IMGSoundEnabled2 = loadImageAtPath("graphics/soundenabled2.png");
	IMGSoundDisabled1 = loadImageAtPath("graphics/sounddisabled1.png");
	IMGSoundDisabled2 = loadImageAtPath("graphics/sounddisabled2.png");


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
	pd->graphics->freeBitmap(IMGMusicEnabled1);
	pd->graphics->freeBitmap(IMGMusicEnabled2);
	pd->graphics->freeBitmap(IMGMusicDisabled1);
	pd->graphics->freeBitmap(IMGMusicDisabled2);
	pd->graphics->freeBitmap(IMGSoundEnabled1);
	pd->graphics->freeBitmap(IMGSoundEnabled2);
	pd->graphics->freeBitmap(IMGSoundDisabled1);
	pd->graphics->freeBitmap(IMGSoundDisabled2);
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
		case 3: 
			OptionMusic = !OptionMusic;
			break;
		case 4:
			OptionSound = !OptionSound;
			break;
		default:
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
		case 3: 
			OptionMusic = !OptionMusic;
			break;
		case 4:
			OptionSound = !OptionSound;
			break;
		default:
			break;
    }
    CAudio_PlaySound(Sounds[SND_SELECT],0);
}

void COptionMenu::NextItem()
{
    Selection++;
    if (Selection == 5)
        Selection = 1;
    CAudio_PlaySound(Sounds[SND_MENU],0);
}

void COptionMenu::PreviousItem()
{
    Selection--;
    if (Selection == 0)
        Selection = 4;
    CAudio_PlaySound(Sounds[SND_MENU],0);
}

// Draw the main menu
void COptionMenu::Draw(LCDBitmap *Surface)
{
    // draw the title screen background
    pd->graphics->drawBitmap(IMGTitleScreen, 0, 0, kBitmapUnflipped);
	const int starty = 50, yspacing = 30;

    if (Selection == 1)
    {
        if(OptionJump)
        {
            pd->graphics->drawBitmap(IMGJumpEnabled1, 40, starty, kBitmapUnflipped);
        }
        else
        {
			pd->graphics->drawBitmap(IMGJumpDisabled1, 40, starty, kBitmapUnflipped);
        }
    }
    else
    {
        if(OptionJump)
        {
			pd->graphics->drawBitmap(IMGJumpEnabled2, 40, starty, kBitmapUnflipped);
        }
        else
        {
            pd->graphics->drawBitmap(IMGJumpDisabled2, 40, starty, kBitmapUnflipped);
        }
    }

    if (Selection == 2)
    {
        switch(OptionDifficulty)
        {
            case VeryEasy:
                pd->graphics->drawBitmap(IMGVeryEasy1, 40, starty + (1 * yspacing) + 4, kBitmapUnflipped);
                break;
            case Easy:
				pd->graphics->drawBitmap(IMGEasy1, 40, starty + (1 * yspacing)+ 4, kBitmapUnflipped);
				break;
            case Hard:
                pd->graphics->drawBitmap(IMGHard1, 40, starty + (1 * yspacing)+ 4, kBitmapUnflipped);
                break;
            case VeryHard:
                pd->graphics->drawBitmap(IMGVeryHard1, 40, starty + (1 * yspacing)+ 4, kBitmapUnflipped);
                break;
        }
    }
    else
    {
        switch(OptionDifficulty)
        {
   			case VeryEasy:
                pd->graphics->drawBitmap(IMGVeryEasy2, 40, starty + (1 * yspacing)+ 4, kBitmapUnflipped);
                break;
            case Easy:
				pd->graphics->drawBitmap(IMGEasy2, 40, starty + (1 * yspacing)+ 4, kBitmapUnflipped);
				break;
            case Hard:
                pd->graphics->drawBitmap(IMGHard2, 40, starty + (1 * yspacing)+ 4, kBitmapUnflipped);
                break;
            case VeryHard:
                pd->graphics->drawBitmap(IMGVeryHard2, 40, starty + (1 * yspacing)+ 4, kBitmapUnflipped);
                break;
        }
    }

	if (Selection == 3)
    {
        if(OptionMusic)
        {
            pd->graphics->drawBitmap(IMGMusicEnabled1, 40, starty + (2 * yspacing), kBitmapUnflipped);
        }
        else
        {
			pd->graphics->drawBitmap(IMGMusicDisabled1, 40, starty + (2 * yspacing), kBitmapUnflipped);
        }
    }
    else
    {
        if(OptionMusic)
        {
			pd->graphics->drawBitmap(IMGMusicEnabled2, 40, starty + (2 * yspacing), kBitmapUnflipped);
        }
        else
        {
            pd->graphics->drawBitmap(IMGMusicDisabled2, 40, starty + (2 * yspacing), kBitmapUnflipped);
        }
    }


	if (Selection == 4)
    {
        if(OptionSound)
        {
            pd->graphics->drawBitmap(IMGSoundEnabled1, 40, starty + (3 * yspacing), kBitmapUnflipped);
        }
        else
        {
			pd->graphics->drawBitmap(IMGSoundDisabled1, 40, starty + (3 * yspacing), kBitmapUnflipped);
        }
    }
    else
    {
        if(OptionSound)
        {
			pd->graphics->drawBitmap(IMGSoundEnabled2, 40, starty + (3 * yspacing), kBitmapUnflipped);
        }
        else
        {
            pd->graphics->drawBitmap(IMGSoundDisabled2, 40, starty + (3 * yspacing), kBitmapUnflipped);
        }
    }
}
