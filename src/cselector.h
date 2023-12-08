#ifndef CSELECTOR_H
#define CSELECTOR_H

#include <SDL/SDL.h>
#include "common.h"
#include "cbord.h"

class CSelector
{
    private:
        SPoint SelectedPoint,CurrentPoint; //variables to hold the current position and the selected position
		SDL_Surface *IMGSelect, *IMGSelectMove;
    public:
        bool HasSelection,Visible;
        CSelector(const int PlayFieldXin,const int PlayFieldYin);
        void Hide();
        void Show();
        void SetPosition(const int PlayFieldXin,const int PlayFieldYin);
        void Select();
        SPoint GetSelection();
        SPoint GetPosition();
        void DeSelect();
        void Draw(SDL_Surface *Surface,CBord *Bord);
        ~CSelector();
};
#endif