#ifndef CMAINMENU_H
#define CMAINMENU_H

#include <SDL/SDL.h>

// Class for the main menu
class CMainMenu
{
    private:
        int Selection; //Holds what we currently selected in the main menu, 1 = new game, 2= credits, 3 = quit
        SDL_Surface *IMGPlay1,*IMGPlay2,*IMGQuit1,*IMGQuit2,*IMGOptions1,*IMGOptions2,*IMGTitleScreen; // the surfaces needed for the graphics inside the game
    public:
        CMainMenu(); // constructor
        void Draw(SDL_Surface *Surface); //draw to a certain surface
        void NextItem(); //Select the next menuitem
        void PreviousItem(); //select the previous menuitem
        int GetSelection() { return Selection; } // return the current selection
        ~CMainMenu(); // destructor
};
#endif