#ifndef HELPERFUNCS_H
#define HELPERFUNCS_H

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

void WriteText(SDL_Surface* Surface,TTF_Font* FontIn,char* Tekst,int NrOfChars,int X,int Y,int YSpacing,SDL_Color ColorIn);

#endif
