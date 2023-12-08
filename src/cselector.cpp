#include <pd_api.h>
#include "pd_helperfuncs.h"
#include "cselector.h"
#include "common.h"
#include "cbord.h"


// constructor, parameters define the starting position
CSelector::CSelector(const int PlayFieldXin,const int PlayFieldYin)
{

	IMGSelect = loadImageAtPath("graphics/select.png");
	IMGSelectMove = loadImageAtPath("graphics/select_move.png");

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
void CSelector::Draw(LCDBitmap *Surface,CBord *Bord)
{
	SMove Move;
	bool JumpFound = false;
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
				pd->graphics->drawBitmap(IMGSelectMove, (SelectedPoint.X-2) * (TileWidth), (SelectedPoint.Y-2) * (TileHeight), kBitmapUnflipped);
				JumpFound = true;
			}
			Move.NewX = SelectedPoint.X + 2;
			if(Bord->ValidMove(Move))
			{
				pd->graphics->drawBitmap(IMGSelectMove, (SelectedPoint.X+2) * (TileWidth), (SelectedPoint.Y-2) * (TileHeight), kBitmapUnflipped);
				JumpFound = true;
			}
			Move.NewY = SelectedPoint.Y + 2;
			if(Bord->ValidMove(Move))
			{
				pd->graphics->drawBitmap(IMGSelectMove, (SelectedPoint.X+2) * (TileWidth), (SelectedPoint.Y+2) * (TileHeight), kBitmapUnflipped);
				JumpFound = true;
			}
			Move.NewX = SelectedPoint.X -2;
			if(Bord->ValidMove(Move))
			{
				pd->graphics->drawBitmap(IMGSelectMove, (SelectedPoint.X+2) * (TileWidth), (SelectedPoint.Y-2) * (TileHeight), kBitmapUnflipped);
				JumpFound = true;
			}
			if (!JumpFound)
			{
				Move.NewX = SelectedPoint.X -1;
				Move.NewY = SelectedPoint.Y -1;
				if(Bord->ValidMove(Move))
					pd->graphics->drawBitmap(IMGSelectMove, (SelectedPoint.X-1) * (TileWidth), (SelectedPoint.Y-1) * (TileHeight), kBitmapUnflipped);
				
				Move.NewX = SelectedPoint.X + 1;
				if(Bord->ValidMove(Move))
					pd->graphics->drawBitmap(IMGSelectMove, (SelectedPoint.X+1) * (TileWidth), (SelectedPoint.Y-1) * (TileHeight), kBitmapUnflipped);
				
				Move.NewY = SelectedPoint.Y + 1;
				if(Bord->ValidMove(Move))
					pd->graphics->drawBitmap(IMGSelectMove, (SelectedPoint.X+1) * (TileWidth), (SelectedPoint.Y+1) * (TileHeight), kBitmapUnflipped);
				
				Move.NewX = SelectedPoint.X -1;
				if(Bord->ValidMove(Move))
					pd->graphics->drawBitmap(IMGSelectMove, (SelectedPoint.X-1) * (TileWidth), (SelectedPoint.Y+1) * (TileHeight), kBitmapUnflipped);
				
			}
			pd->graphics->drawBitmap(IMGSelectMove, (CurrentPoint.X) * (TileWidth), (CurrentPoint.Y) * (TileHeight), kBitmapUnflipped);
				
		}
		else
		{
			pd->graphics->drawBitmap(IMGSelect, (CurrentPoint.X) * (TileWidth), (CurrentPoint.Y) * (TileHeight), kBitmapUnflipped);
		}
	}
}

CSelector::~CSelector()
{
	pd->graphics->freeBitmap(IMGSelect);
	pd->graphics->freeBitmap(IMGSelectMove);
}
