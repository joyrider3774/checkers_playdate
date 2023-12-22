#include <cstring>
#include <vector>
#include <pd_api.h>
#include "game.h"
#include "cbord.h"
#include "common.h"
#include "cselector.h"
#include "ccomputerai.h"
#include "pd_helperfuncs.h"
#include "sdl_helpertypes.h"
#include "caudio.h"

using namespace std;

bool confirmingQuit = false;

bool MoveInList(SMove Move,vector<SMove> List)
{
    vector<SMove>::iterator iter=List.begin();
    while(iter != List.end())
    {
        if((Move.NewX == iter->NewX) && (Move.NewY == iter->NewY) &&
           (Move.X == iter->X) && (Move.Y == iter->Y) && (Move.Player == iter->Player))
            return true;
        iter++;
    }
    return false;
}

void LoadGameGraphics()
{
    BlackSquare = loadImageAtPath("graphics/black");
    WhiteSquare = loadImageAtPath("graphics/white");
    BluePawn = loadImageAtPath("graphics/blue");
    RedPawn = loadImageAtPath("graphics/red");
    RedKing = loadImageAtPath("graphics/redking");
    BlueKing = loadImageAtPath("graphics/blueking");
    IMGPanel = loadImageAtPath("graphics/panel");
    IMGPlayer = loadImageAtPath("graphics/player");
    IMGCpu = loadImageAtPath("graphics/cpu");
    IMGPlayerWins = loadImageAtPath("graphics/playerwins");
    IMGCpuWins = loadImageAtPath("graphics/cpuwins");
    IMGDraw = loadImageAtPath("graphics/draw");
	IMGConfirmQuit = loadImageAtPath("graphics/confirmquit");
}

void UnLoadGameGraphics()
{
    pd->graphics->freeBitmap(BlackSquare);
    pd->graphics->freeBitmap(WhiteSquare);
    pd->graphics->freeBitmap(BluePawn);
    pd->graphics->freeBitmap(RedPawn);
    pd->graphics->freeBitmap(RedKing);
    pd->graphics->freeBitmap(BlueKing);
    pd->graphics->freeBitmap(IMGPanel);
    pd->graphics->freeBitmap(IMGPlayer);
    pd->graphics->freeBitmap(IMGCpu);
    pd->graphics->freeBitmap(IMGPlayerWins);
    pd->graphics->freeBitmap(IMGCpuWins);
    pd->graphics->freeBitmap(IMGDraw);
	pd->graphics->freeBitmap(IMGConfirmQuit);
}

SMove Move;
GameResult Result;
int NumHumanPieces,NumComputerPieces,X,Y;
vector <SMove> Moves;
vector<SMove> TakeMovesList,NextTakeMoveList;
ComputerAi *Ai;
CBord *Bord;
CSelector* Selector;
int PlayerNr;
size_t StartFor;
bool HumanAnotherMove=false;

bool GameInit()
{
	LoadGameGraphics();
	Result = Playing;
	NumHumanPieces=0;
	NumComputerPieces=0;
	Ai = new ComputerAi(Difficulty,JumpHeuristicEnabled);
	Bord = new CBord();
	Selector = new CSelector(4,3);
	PlayerNr=1;
	HumanAnotherMove=false;
	MadeMoveList.clear();
	CAudio_PlaySound(Sounds[SND_START],0);
	confirmingQuit = false;
	pd->graphics->setBackgroundColor(kColorBlack);
	return true;
}

void Game()
{

	if (GameState == GSGameInit)
	{
		if(GameInit())
			GameState = GSGame; 
	}

	if (GameState == GSGame)
	{
		AiStatus aiStatus = Ai->Update();
		if(aiStatus == AiCalculating)
			return;

		if(Input->KeyboardPushed[SDLK_b])
		{
			if((Result == Playing) && (PlayerNr = Human))
			{
				if(!confirmingQuit)
					CAudio_PlaySound(Sounds[SND_SELECT],0);
				confirmingQuit = !confirmingQuit;
			}
		}
		
		if(!confirmingQuit && Input->KeyboardPushed[SDLK_LEFT])
			if ((Result == Playing) && (!Selector->HasSelection))
		    	Selector->SetPosition(Selector->GetPosition().X-1,Selector->GetPosition().Y);
		
		if(!confirmingQuit && Input->KeyboardPushed[SDLK_RIGHT])
			if ((Result == Playing) && (!Selector->HasSelection))
		    	Selector->SetPosition(Selector->GetPosition().X+1,Selector->GetPosition().Y);

		if(!confirmingQuit && Input->KeyboardPushed[SDLK_DOWN])
        	if ((Result == Playing) && (!Selector->HasSelection))
		    	Selector->SetPosition(Selector->GetPosition().X,Selector->GetPosition().Y+1);
		
		if(!confirmingQuit && Input->KeyboardPushed[SDLK_UP])
			if ((Result == Playing) && (!Selector->HasSelection))
		    	Selector->SetPosition(Selector->GetPosition().X,Selector->GetPosition().Y-1);


		if(!confirmingQuit && Input->Ready() && Input->KeyboardHeld[SDLK_UP] && Input->KeyboardHeld[SDLK_RIGHT])
			if(Selector->HasSelection)
			{
				Move.Player = PlayerNr;
				Move.DeletedPiece = -1;
				Move.KingX = -1;
				Move.KingY = -1;
				Move.X = Selector->GetSelection().X;
				Move.Y = Selector->GetSelection().Y;
				Move.NewX = Selector->GetSelection().X +2 ;
				Move.NewY = Selector->GetSelection().Y -2 ;
				if (Bord->ValidMove(Move))
				{

					if(!HumanAnotherMove)
					{
						TakeMovesList = Bord->GetSingleTakeMoves(Human);
						if(TakeMovesList.size() > 0)
						{
							if(MoveInList(Move,TakeMovesList))
							{
								Bord->ApplyMove(Move,true);
								Input->Delay();
								Input->Reset();
								MadeMoveList.push_back(Move);
								Selector->SetPosition(Selector->GetSelection().X +2,Selector->GetSelection().Y -2);
								if((Move.KingX == -1) && (Move.KingY == -1))
								{
									NextTakeMoveList=Bord->GetSingleTakeMoves(Human,Move.NewX,Move.NewY);
									if(NextTakeMoveList.size() > 0)
									{
										HumanAnotherMove = true;
										Selector->Select();
									}
									else
									{
										PlayerNr= Computer;
										HumanAnotherMove = false;
										Selector->HasSelection = false;
										Selector->Hide();
									}
								}
								else
								{
									PlayerNr= Computer;
									HumanAnotherMove = false;
									Selector->HasSelection = false;
									Selector->Hide();
								}
							}
							else
							{
								Input->Delay();
								Input->Reset();
								CAudio_PlaySound(Sounds[SND_WRONG],0);
							}
						}
						else
						{
							Bord->ApplyMove(Move,true);
							Input->Delay();
							Input->Reset();
							MadeMoveList.push_back(Move);
							PlayerNr= Computer;
							Selector->HasSelection = false;
							Selector->Hide();
						}
					}
					else
					{
						if( MoveInList(Move,NextTakeMoveList))
						{
							Bord->ApplyMove(Move,true);
							Input->Delay();
							Input->Reset();
							MadeMoveList.push_back(Move);
							Selector->SetPosition(Selector->GetSelection().X +2,Selector->GetSelection().Y -2);
							Selector->HasSelection = false;
							if((Move.KingX == -1) && (Move.KingY == -1))
							{
								NextTakeMoveList=Bord->GetSingleTakeMoves(Human,Move.NewX,Move.NewY);
								if(NextTakeMoveList.size() > 0)
								{
									HumanAnotherMove = true;
									Selector->Select();
								}
								else
								{
									PlayerNr= Computer;
									HumanAnotherMove = false;
									Selector->HasSelection = false;
									Selector->Hide();
								}
							}
							else
							{
								PlayerNr= Computer;
								HumanAnotherMove = false;
								Selector->HasSelection = false;
								Selector->Hide();
							}
						}
					}
				}
				else
				{
					TakeMovesList = Bord->GetSingleTakeMoves(Human);
					if (TakeMovesList.size() == 0)
					{
						Move.NewX = Selector->GetSelection().X+1;
						Move.NewY = Selector->GetSelection().Y-1;
						if (Bord->ValidMove(Move))
						{
							Bord->ApplyMove(Move,true);
							Input->Delay();
							Input->Reset();
							MadeMoveList.push_back(Move);
							Selector->SetPosition( Selector->GetSelection().X+1,Selector->GetSelection().Y-1);
							Selector->HasSelection = false;
							Selector->Hide();
							PlayerNr = Computer;
						}
						else
						{
							Input->Delay();
							Input->Reset();
							CAudio_PlaySound(Sounds[SND_WRONG],0);
						}
					}
					else
					{
						Input->Delay();
						Input->Reset();
						CAudio_PlaySound(Sounds[SND_WRONG],0);
					}
				}

			}

		if(!confirmingQuit && Input->Ready() && Input->KeyboardHeld[SDLK_UP] && Input->KeyboardHeld[SDLK_LEFT])
			if(Selector->HasSelection)
			{
				Move.Player = PlayerNr;
				Move.DeletedPiece = -1;
				Move.KingX = -1;
				Move.KingY = -1;
				Move.X = Selector->GetSelection().X;
				Move.Y = Selector->GetSelection().Y;
				Move.NewX = Selector->GetSelection().X -2 ;
				Move.NewY = Selector->GetSelection().Y -2 ;
				if (Bord->ValidMove(Move))
				{

					if(!HumanAnotherMove)
					{
						TakeMovesList = Bord->GetSingleTakeMoves(Human);
						if(TakeMovesList.size() > 0)
						{
							if(MoveInList(Move,TakeMovesList))
							{
								Bord->ApplyMove(Move,true);
								Input->Delay();
								Input->Reset();
								MadeMoveList.push_back(Move);
								Selector->SetPosition(Selector->GetSelection().X -2,Selector->GetSelection().Y -2);
								if((Move.KingX == -1) && (Move.KingY == -1))
								{
									NextTakeMoveList=Bord->GetSingleTakeMoves(Human,Move.NewX,Move.NewY);
									if(NextTakeMoveList.size() > 0)
									{
										HumanAnotherMove = true;
										Selector->Select();
									}
									else
									{
										PlayerNr= Computer;
										HumanAnotherMove = false;
										Selector->HasSelection = false;
										Selector->Hide();
									}
								}
								else
								{
									PlayerNr= Computer;
									HumanAnotherMove = false;
									Selector->HasSelection = false;
									Selector->Hide();
								}
							}
							else
							{
								Input->Delay();
								Input->Reset();
								CAudio_PlaySound(Sounds[SND_WRONG],0);
							}
						}
						else
						{
							Bord->ApplyMove(Move,true);
							Input->Delay();
							Input->Reset();
							MadeMoveList.push_back(Move);
							PlayerNr= Computer;
							Selector->HasSelection = false;
							Selector->Hide();
						}
					}
					else
					{
						if( MoveInList(Move,NextTakeMoveList))
						{
							Bord->ApplyMove(Move,true);
							Input->Delay();
							Input->Reset();
							MadeMoveList.push_back(Move);
							Selector->SetPosition(Selector->GetSelection().X -2,Selector->GetSelection().Y -2);
							Selector->HasSelection = false;
							if((Move.KingX == -1) && (Move.KingY == -1))
							{
								NextTakeMoveList=Bord->GetSingleTakeMoves(Human,Move.NewX,Move.NewY);
								if(NextTakeMoveList.size() > 0)
								{
									HumanAnotherMove = true;
									Selector->Select();
								}
								else
								{
									PlayerNr= Computer;
									HumanAnotherMove = false;
									Selector->HasSelection = false;
									Selector->Hide();
								}
							}
							else
							{
								PlayerNr= Computer;
								HumanAnotherMove = false;
								Selector->HasSelection = false;
								Selector->Hide();
							}
						}
					}
				}
				else
				{
					TakeMovesList = Bord->GetSingleTakeMoves(Human);
					if (TakeMovesList.size() == 0)
					{
						Move.NewX = Selector->GetSelection().X-1;
						Move.NewY = Selector->GetSelection().Y-1;
						if (Bord->ValidMove(Move))
						{
							Bord->ApplyMove(Move,true);
							Input->Delay();
							Input->Reset();
							MadeMoveList.push_back(Move);
							Selector->SetPosition( Selector->GetSelection().X-1,Selector->GetSelection().Y-1);
							Selector->HasSelection = false;
							Selector->Hide();
							PlayerNr = Computer;
						}
						else
						{
							Input->Delay();
							Input->Reset();
							CAudio_PlaySound(Sounds[SND_WRONG],0);
						}
					}
					else
					{
						Input->Delay();
						Input->Reset();
						CAudio_PlaySound(Sounds[SND_WRONG],0);
					}
				}

			}

		if(!confirmingQuit && Input->Ready() && Input->KeyboardHeld[SDLK_DOWN] && Input->KeyboardHeld[SDLK_LEFT])
			if(Selector->HasSelection)
			{
				Move.Player = PlayerNr;
				Move.DeletedPiece = -1;
				Move.KingX = -1;
				Move.KingY = -1;
				Move.X = Selector->GetSelection().X;
				Move.Y = Selector->GetSelection().Y;
				Move.NewX = Selector->GetSelection().X -2 ;
				Move.NewY = Selector->GetSelection().Y +2 ;
				if (Bord->ValidMove(Move))
				{

					if(!HumanAnotherMove)
					{
						TakeMovesList = Bord->GetSingleTakeMoves(Human);
						if(TakeMovesList.size() > 0)
						{
							if(MoveInList(Move,TakeMovesList))
							{
								Bord->ApplyMove(Move,true);
								Input->Delay();
								Input->Reset();
								MadeMoveList.push_back(Move);
								Selector->SetPosition(Selector->GetSelection().X -2,Selector->GetSelection().Y +2);
								if((Move.KingX == -1) && (Move.KingY == -1))
								{
									NextTakeMoveList=Bord->GetSingleTakeMoves(Human,Move.NewX,Move.NewY);
									if(NextTakeMoveList.size() > 0)
									{
										HumanAnotherMove = true;
										Selector->Select();
									}
									else
									{
										PlayerNr= Computer;
										HumanAnotherMove = false;
										Selector->HasSelection = false;
										Selector->Hide();
									}
								}
								else
								{
									PlayerNr= Computer;
									HumanAnotherMove = false;
									Selector->HasSelection = false;
									Selector->Hide();
								}
							}
							else
							{
								Input->Delay();
								Input->Reset();
								CAudio_PlaySound(Sounds[SND_WRONG],0);
							}
						}
						else
						{
							Bord->ApplyMove(Move,true);
							Input->Delay();
							Input->Reset();
							MadeMoveList.push_back(Move);
							PlayerNr= Computer;
							Selector->HasSelection = false;
							Selector->Hide();
						}
					}
					else
					{
						if( MoveInList(Move,NextTakeMoveList))
						{
							Bord->ApplyMove(Move,true);
							Input->Delay();
							Input->Reset();
							MadeMoveList.push_back(Move);
							Selector->SetPosition(Selector->GetSelection().X -2,Selector->GetSelection().Y +2);
							Selector->HasSelection = false;
							if((Move.KingX == -1) && (Move.KingY == -1))
							{
								NextTakeMoveList=Bord->GetSingleTakeMoves(Human,Move.NewX,Move.NewY);
								if(NextTakeMoveList.size() > 0)
								{
									HumanAnotherMove = true;
									Selector->Select();
								}
								else
								{
									PlayerNr= Computer;
									HumanAnotherMove = false;
									Selector->HasSelection = false;
									Selector->Hide();
								}
							}
							else
							{
								PlayerNr= Computer;
								HumanAnotherMove = false;
								Selector->HasSelection = false;
								Selector->Hide();
							}
						}
					}
				}
				else
				{
					TakeMovesList = Bord->GetSingleTakeMoves(Human);
					if (TakeMovesList.size() == 0)
					{
						Move.NewX = Selector->GetSelection().X-1;
						Move.NewY = Selector->GetSelection().Y+1;
						if (Bord->ValidMove(Move))
						{
							Bord->ApplyMove(Move,true);
							Input->Delay();
							Input->Reset();
							MadeMoveList.push_back(Move);
							Selector->SetPosition( Selector->GetSelection().X-1,Selector->GetSelection().Y+1);
							Selector->HasSelection = false;
							Selector->Hide();
							PlayerNr = Computer;
						}
						else
						{
							Input->Delay();
							Input->Reset();
							CAudio_PlaySound(Sounds[SND_WRONG],0);
						}
					}
					else
					{
						Input->Delay();
						Input->Reset();
						CAudio_PlaySound(Sounds[SND_WRONG],0);
					}
				}

			}

		if(!confirmingQuit && Input->Ready() && Input->KeyboardHeld[SDLK_DOWN] && Input->KeyboardHeld[SDLK_RIGHT])
			if(Selector->HasSelection)
			{
				Move.Player = PlayerNr;
				Move.DeletedPiece = -1;
				Move.KingX = -1;
				Move.KingY = -1;
				Move.X = Selector->GetSelection().X;
				Move.Y = Selector->GetSelection().Y;
				Move.NewX = Selector->GetSelection().X +2 ;
				Move.NewY = Selector->GetSelection().Y +2 ;
				if (Bord->ValidMove(Move))
				{
					if(!HumanAnotherMove)
					{
						TakeMovesList = Bord->GetSingleTakeMoves(Human);
						if(TakeMovesList.size() > 0)
						{
							if(MoveInList(Move,TakeMovesList))
							{
								Bord->ApplyMove(Move,true);
								Input->Delay();
								Input->Reset();
								MadeMoveList.push_back(Move);
								Selector->SetPosition(Selector->GetSelection().X +2,Selector->GetSelection().Y +2);
								if((Move.KingX == -1) && (Move.KingY == -1))
								{
									NextTakeMoveList=Bord->GetSingleTakeMoves(Human,Move.NewX,Move.NewY);
									if(NextTakeMoveList.size() > 0)
									{
										HumanAnotherMove = true;
										Selector->Select();
									}
									else
									{
										PlayerNr= Computer;
										HumanAnotherMove = false;
										Selector->HasSelection = false;
										Selector->Hide();
									}
								}
								else
								{
									PlayerNr= Computer;
									HumanAnotherMove = false;
									Selector->HasSelection = false;
									Selector->Hide();
								}
							}
							else
							{
								Input->Delay();
								Input->Reset();
								CAudio_PlaySound(Sounds[SND_WRONG],0);
							}
						}
						else
						{
							Bord->ApplyMove(Move,true);
							Input->Delay();
							Input->Reset();
							MadeMoveList.push_back(Move);
							PlayerNr= Computer;
							Selector->HasSelection = false;
							Selector->Hide();
						}
					}
					else
					{
						if( MoveInList(Move,NextTakeMoveList))
						{
							Bord->ApplyMove(Move,true);
							Input->Delay();
							Input->Reset();
							MadeMoveList.push_back(Move);
							Selector->SetPosition(Selector->GetSelection().X +2,Selector->GetSelection().Y +2);
							Selector->HasSelection = false;
							if((Move.KingX == -1) && (Move.KingY == -1))
							{
								NextTakeMoveList=Bord->GetSingleTakeMoves(Human,Move.NewX,Move.NewY);
								if(NextTakeMoveList.size() > 0)
								{
									HumanAnotherMove = true;
									Selector->Select();
								}
								else
								{
									PlayerNr= Computer;
									HumanAnotherMove = false;
									Selector->HasSelection = false;
									Selector->Hide();
								}
							}
							else
							{
								PlayerNr= Computer;
								HumanAnotherMove = false;
								Selector->HasSelection = false;
								Selector->Hide();
							}
						}
					}
				}
				else
				{
					TakeMovesList = Bord->GetSingleTakeMoves(Human);
					if (TakeMovesList.size() == 0)
					{
						Move.NewX = Selector->GetSelection().X+1;
						Move.NewY = Selector->GetSelection().Y+1;
						if (Bord->ValidMove(Move))
						{
							Bord->ApplyMove(Move,true);
							Input->Delay();
							Input->Reset();
							MadeMoveList.push_back(Move);
							Selector->SetPosition( Selector->GetSelection().X+1,Selector->GetSelection().Y+1);
							Selector->HasSelection = false;
							Selector->Hide();
							PlayerNr = Computer;
						}
						else
						{
							Input->Delay();
							Input->Reset();
							CAudio_PlaySound(Sounds[SND_WRONG],0);
						}
					}
					else
					{
						Input->Delay();
						Input->Reset();
						CAudio_PlaySound(Sounds[SND_WRONG],0);
					}
				}
			}

		if(Input->KeyboardPushed[SDLK_a])
		{
			if (Result == Playing)
			{
				if (confirmingQuit)
				{
					GameState = GSTitleScreenInit;
				}
				else
				{
					if (!Selector->HasSelection && ((Bord->GetBordValue(Selector->GetPosition().X,Selector->GetPosition().Y) == HumanPawn) || (Bord->GetBordValue(Selector->GetPosition().X,Selector->GetPosition().Y) == HumanKing)))
					{
						CAudio_PlaySound(Sounds[SND_SELECT],0);
						Selector->Select();
					}
					else
						Selector->DeSelect();
				}
			}
			else
				GameState = GSTitleScreenInit;
		}
		pd->graphics->clear(kColorBlack);
		Bord->Draw(NULL);
		if (!Bord->GameEnded())
			Selector->Draw(NULL, Bord);

		pd->graphics->drawBitmap(IMGPanel, 240, 0, kBitmapUnflipped);
		if(PlayerNr == Human)
			pd->graphics->drawBitmap(IMGPlayer, 245, 5, kBitmapUnflipped);
		else
			pd->graphics->drawBitmap(IMGCpu, 245, 5, kBitmapUnflipped);
		if(Result == Playing)
			if(confirmingQuit)
				pd->graphics->drawBitmap(IMGConfirmQuit, 4, 76, kBitmapUnflipped);
		if(Result == Draw)
			pd->graphics->drawBitmap(IMGDraw, 4, 76, kBitmapUnflipped);
		if(Result == PlayerWin)
			pd->graphics->drawBitmap(IMGPlayerWins, 4, 76, kBitmapUnflipped);
		if(Result == CPUWin)
			pd->graphics->drawBitmap(IMGCpuWins, 4, 76, kBitmapUnflipped);


		if((int)(MadeMoveList.size()-8) < 0)
			StartFor = 0;
		else
			StartFor = MadeMoveList.size()-8;
		for(size_t i=StartFor;i<MadeMoveList.size();i++)
		{
			char *Msg;
			pd->system->formatString(&Msg,"%d,%d > %d,%d",MadeMoveList[i].X+1,MadeMoveList[i].Y+1,MadeMoveList[i].NewX+1,MadeMoveList[i].NewY+1);
			if(MadeMoveList[i].Player == Human)
			{
				pd->graphics->fillRect(247,(int)(80+((i-StartFor)*18)), 66, 18, kColorWhite);
				drawTextColor(true, NULL, font, Msg, strlen(Msg), kASCIIEncoding, 247, 80+((i-StartFor)*18), kColorBlack, false);
			}    
			else
			{
				pd->graphics->fillRect(247,(int)(80+((i-StartFor)*18)), 66, 18, kColorBlack);
				drawTextColor(true, NULL, font, Msg, strlen(Msg), kASCIIEncoding, 247, 80+((i-StartFor)*18), kColorWhite, false);
			}
			pd->system->realloc(Msg, 0);
		}
		
		if(PlayerNr==Computer && !Bord->GameEnded())
		{
			if(aiStatus == AiIdle)
				Ai->MakeMove(Bord,Computer);
			else if (aiStatus == AiHasResult)
			{
				if (Ai->madeMove)
				{
					PlayerNr = Human;
					Selector->Show();
				}
				else
				{
					Result = PlayerWin;
					CAudio_PlaySound(Sounds[SND_WINNER],0);
				}
			}
		}
		if((Result==Playing) && (PlayerNr == Human) && (Bord->GetTakeMoves(Human).size() == 0) && (Bord->GetNormalMoves(Human).size() == 0))
		{
			Selector->Hide();
			Result = CPUWin;
			CAudio_PlaySound(Sounds[SND_LOSER],0);
		}
		if(Result == Playing)
		{
			if(Bord->GameEnded())
			{
				for(Y=0;Y<NrOfRows;Y++)
					for(X=0;X<NrOfCols;X++)
						if(Bord->GetBordValue(X,Y) != Empty)
						{
							if((Bord->GetBordValue(X,Y) == HumanKing) || (Bord->GetBordValue(X,Y) == HumanPawn))
							{
								NumHumanPieces++;
							}
							else
							{
								NumComputerPieces++;
							}
						}
				if((NumHumanPieces == 1) && (NumComputerPieces == 1))
				{
					Result = Draw;
					CAudio_PlaySound(Sounds[SND_DRAW],0);
				}
				else
					if(NumHumanPieces > 0)
					{
						Result = PlayerWin;
						CAudio_PlaySound(Sounds[SND_WINNER],0);
					}
					else
						if(NumComputerPieces > 0)
						{
							Result = CPUWin;
							CAudio_PlaySound(Sounds[SND_LOSER],0);
						}
			}
		}
	}
	if(GameState != GSGame)
	{
		UnLoadGameGraphics();
		delete Selector;
		delete Ai;
		delete Bord;
	}
}