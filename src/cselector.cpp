#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "cselector.h"
#include "common.h"
#include "cbord.h"


// constructor, parameters define the starting position
CSelector::CSelector(const int PlayFieldXin,const int PlayFieldYin)
{
	SDL_Surface* Tmp;
	Tmp = IMG_Load("./Source/graphics/select.png");
	IMGSelect = SDL_DisplayFormatAlpha(Tmp);
	SDL_FreeSurface(Tmp);

	Tmp = IMG_Load("./Source/graphics/select_move.png");
	IMGSelectMove = SDL_DisplayFormatAlpha(Tmp);
	SDL_FreeSurface(Tmp);


	SelectedPoint.X = -1; // there's no selection set it 0
	SelectedPoint.Y = -1;
	CurrentPoint.X = PlayFieldXin; // set current position
	CurrentPoint.Y = PlayFieldYin;
	HasSelection = false; // No selection when it's created
	Visible = true;
}

// Will set the position only if it lays within the board boundary
void CSelector::Hide()
{
	Visible = false;
}

void CSelector::Show()
{
	Visible = true;
}

void CSelector::SetPosition(const int PlayFieldXin,const int PlayFieldYin)
{
	// check if the new position is inside the board, if so set the new position as the currentposition
	if ((PlayFieldYin>=0) && (PlayFieldYin < NrOfRows) && (PlayFieldXin >= 0) && (PlayFieldXin < NrOfCols))
	{
		CurrentPoint.X = PlayFieldXin;
		CurrentPoint.Y = PlayFieldYin;
	}
}

// Select will set the current position as the selected position
void CSelector::Select()
{
	SelectedPoint.X = CurrentPoint.X;
	SelectedPoint.Y = CurrentPoint.Y;
	HasSelection = true;
}

// return the selected position
SPoint CSelector::GetSelection()
{
	return SelectedPoint;
}

// return the current position
SPoint CSelector::GetPosition()
{
	return CurrentPoint;
}

void CSelector::DeSelect()
{
	HasSelection = false;
}

// Draw the blue box on the current position, with the offsets in mind
void CSelector::Draw(SDL_Surface *Surface,CBord *Bord)
{
	SMove Move;
	bool JumpFound = false;
	SDL_Rect Rect;
	if (Visible)
	{
		if(HasSelection)
		{
			Move.Player = Human;
			Move.KingX = -1;
			Move.KingY = -1;
			Move.X = SelectedPoint.X;
			Move.Y = SelectedPoint.Y;
			Move.NewX = SelectedPoint.X -2;
			Move.NewY = SelectedPoint.Y -2;
			Move.DeletedPiece = -1;
			if(Bord->ValidMove(Move))
			{
				Rect.x = (SelectedPoint.X-2) * (TileWidth);
				Rect.y = (SelectedPoint.Y-2) * (TileHeight);
				Rect.w = TileWidth;
				Rect.h = TileHeight;
				SDL_BlitSurface(IMGSelectMove, NULL, Surface, &Rect);
				JumpFound = true;
			}
			Move.NewX = SelectedPoint.X + 2;
			if(Bord->ValidMove(Move))
			{
				Rect.x = (SelectedPoint.X+2) * (TileWidth);
				Rect.y = (SelectedPoint.Y-2) * (TileHeight);
				Rect.w = TileWidth;
				Rect.h = TileHeight;
				SDL_BlitSurface(IMGSelectMove, NULL, Surface, &Rect);
				JumpFound = true;
			}
			Move.NewY = SelectedPoint.Y + 2;
			if(Bord->ValidMove(Move))
			{
				Rect.x = (SelectedPoint.X+2) * (TileWidth);
				Rect.y = (SelectedPoint.Y+2) * (TileHeight);
				Rect.w = TileWidth;
				Rect.h = TileHeight;
				SDL_BlitSurface(IMGSelectMove, NULL, Surface, &Rect);
				JumpFound = true;
			}
			Move.NewX = SelectedPoint.X -2;
			if(Bord->ValidMove(Move))
			{
				Rect.x = (SelectedPoint.X-2) * (TileWidth);
				Rect.y = (SelectedPoint.Y+2) * (TileHeight);
				Rect.w = TileWidth;
				Rect.h = TileHeight;
				SDL_BlitSurface(IMGSelectMove, NULL, Surface, &Rect);
				JumpFound = true;
			}
			if (!JumpFound)
			{
				Move.NewX = SelectedPoint.X -1;
				Move.NewY = SelectedPoint.Y -1;
				if(Bord->ValidMove(Move))
				{
					Rect.x = (SelectedPoint.X-1) * (TileWidth);
					Rect.y = (SelectedPoint.Y-1) * (TileHeight);
					Rect.w = TileWidth;
					Rect.h = TileHeight;
					SDL_BlitSurface(IMGSelectMove, NULL, Surface, &Rect);
				}
				Move.NewX = SelectedPoint.X + 1;
				if(Bord->ValidMove(Move))
				{
					Rect.x = (SelectedPoint.X+1) * (TileWidth);
					Rect.y = (SelectedPoint.Y-1) * (TileHeight);
					Rect.w = TileWidth;
					Rect.h = TileHeight;
					SDL_BlitSurface(IMGSelectMove, NULL, Surface, &Rect);
				}
				Move.NewY = SelectedPoint.Y + 1;
				if(Bord->ValidMove(Move))
				{
					Rect.x = (SelectedPoint.X+1) * (TileWidth);
					Rect.y = (SelectedPoint.Y+1) * (TileHeight);
					Rect.w = TileWidth;
					Rect.h = TileHeight;
					SDL_BlitSurface(IMGSelectMove, NULL, Surface, &Rect);
				}
				Move.NewX = SelectedPoint.X -1;
				if(Bord->ValidMove(Move))
				{
					Rect.x = (SelectedPoint.X-1) * (TileWidth);
					Rect.y = (SelectedPoint.Y+1) * (TileHeight);
					Rect.w = TileWidth;
					Rect.h = TileHeight;
					SDL_BlitSurface(IMGSelectMove, NULL, Surface, &Rect);
				}
			}
			Rect.x = (CurrentPoint.X) * (TileWidth);
			Rect.y = (CurrentPoint.Y) * (TileHeight);
			Rect.w = TileWidth;
			Rect.h = TileHeight;
			SDL_BlitSurface(IMGSelectMove, NULL, Surface, &Rect);
		}
		else
		{
			Rect.x = (CurrentPoint.X) * (TileWidth);
			Rect.y = (CurrentPoint.Y) * (TileHeight);
			Rect.w = TileWidth;
			Rect.h = TileHeight;
			SDL_BlitSurface(IMGSelect, NULL, Surface, &Rect);
		}
	}
}

CSelector::~CSelector()
{
	SDL_FreeSurface(IMGSelect);
	SDL_FreeSurface(IMGSelectMove);
}
