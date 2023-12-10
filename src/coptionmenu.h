#ifndef COPTIONMENU_H
#define COPTIONMENU_H

#include <pd_api.h>

class COptionMenu
{
    private:
        bool OptionJump, OptionSound, OptionMusic;
        int Selection,OptionDifficulty;
        LCDBitmap *IMGVeryEasy1,*IMGVeryEasy2,*IMGEasy1,*IMGEasy2,*IMGHard1,*IMGHard2,*IMGVeryHard1,*IMGVeryHard2,
			*IMGJumpEnabled1,*IMGJumpEnabled2,*IMGJumpDisabled1,*IMGJumpDisabled2,*IMGTitleScreen,
			*IMGMusicEnabled1, *IMGMusicEnabled2, *IMGMusicDisabled1, *IMGMusicDisabled2,
			*IMGSoundEnabled1, *IMGSoundEnabled2, *IMGSoundDisabled1, *IMGSoundDisabled2; // the surfaces needed for the graphics inside the game
    public:
        COptionMenu(int Difficulty,bool JumpEnabled, bool MusicEnabled, bool SoundEnabled); // constructor
        void Draw(LCDBitmap *Surface); //draw to a certain surface
        void NextItem(); //Select the next menuitem
        void PreviousItem(); //select the previous menuitem
        void NextOption();
        void PreviousOption();
        int GetOptionDifficulty() {return OptionDifficulty;}
        bool GetOptionJump(){return OptionJump;}
		bool GetOptionSound(){return OptionSound;}
		bool GetOptionMusic(){return OptionMusic;}
        int GetSelection() { return Selection; } // return the current selection
        ~COptionMenu(); // destructor
};

#endif