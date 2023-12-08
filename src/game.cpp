#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "game.h"
#include "cbord.h"
#include "common.h"
#include "cselector.h"
#include "ccomputerai.h"
#include "helperfuncs.h"

using namespace std;

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
    SDL_Surface *Tmp;
    Tmp = IMG_Load("./Source/graphics/black.png");
    BlackSquare = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);
    Tmp = IMG_Load("./Source/graphics/white.png");
    WhiteSquare = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);
    Tmp = IMG_Load("./Source/graphics/blue.png");
    BluePawn = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);
    Tmp = IMG_Load("./Source/graphics/red.png");
    RedPawn = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);
    Tmp = IMG_Load("./Source/graphics/redking.png");
    RedKing = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);
    Tmp = IMG_Load("./Source/graphics/blueking.png");
    BlueKing = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);
    Tmp = IMG_Load("./Source/graphics/panel.png");
    IMGPanel = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);
    Tmp = IMG_Load("./Source/graphics/player.png");
    IMGPlayer = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);
    Tmp = IMG_Load("./Source/graphics/cpu.png");
    IMGCpu = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);
    Tmp = IMG_Load("./Source/graphics/playerwins.png");
    IMGPlayerWins = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);
    Tmp = IMG_Load("./Source/graphics/cpuwins.png");
    IMGCpuWins = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);
    Tmp = IMG_Load("./Source/graphics/draw.png");
    IMGDraw = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);
}

void UnLoadGameGraphics()
{
    SDL_FreeSurface(BlackSquare);
    SDL_FreeSurface(WhiteSquare);
    SDL_FreeSurface(BluePawn);
    SDL_FreeSurface(RedPawn);
    SDL_FreeSurface(RedKing);
    SDL_FreeSurface(BlueKing);
    SDL_FreeSurface(IMGPanel);
    SDL_FreeSurface(IMGPlayer);
    SDL_FreeSurface(IMGCpu);
    SDL_FreeSurface(IMGPlayerWins);
    SDL_FreeSurface(IMGCpuWins);
    SDL_FreeSurface(IMGDraw);
}

void Game()
{
   SDL_Event Event;
   SDL_Color Color1 = {255,255,255,255},Color2={0,0,0,255};
   char Msg[300];
   SDL_Rect PanelRect,PlayerRect,ResultPanel;
   SMove Move;
   GameResult Result = Playing;
   int NumHumanPieces=0,NumComputerPieces=0,X,Y;
   vector <SMove> Moves;
   vector<SMove> TakeMovesList,NextTakeMoveList;
   ComputerAi *Ai = new ComputerAi(Difficulty,JumpHeuristicEnabled);
   CBord *Bord = new CBord();
   CSelector* Selector = new CSelector(4,3);
   int PlayerNr=1;
   size_t StartFor;
   bool HumanAnotherMove=false;
   LoadGameGraphics();
   PanelRect.x = 240;
   PanelRect.y = 0;
   PanelRect.w = IMGPanel->w;
   PanelRect.h = IMGPanel->h;
   PlayerRect.x = 245;
   PlayerRect.y = 5;
   PlayerRect.w = IMGPlayer->w;
   PlayerRect.h = IMGPlayer->h;
   ResultPanel.x = 4;
   ResultPanel.y = 76;
   ResultPanel.w = IMGDraw->w;
   ResultPanel.h = IMGDraw->h;
   MadeMoveList.clear();
   if (GlobalSoundEnabled)
        Mix_PlayChannel(-1,Sounds[SND_START],0);
   while (GameState == GSGame)
   {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_KEYDOWN)
                switch(Event.key.keysym.sym)
                {
                    case SDLK_b:
                        GameState = GSTitleScreen;
                        break;
                    case SDLK_KP4:
                        if ((Result == Playing) && (!Selector->HasSelection))
                            Selector->SetPosition(Selector->GetPosition().X-1,Selector->GetPosition().Y);
                        break;
                    case SDLK_KP6:
                        if ((Result == Playing) && (!Selector->HasSelection))
                            Selector->SetPosition(Selector->GetPosition().X+1,Selector->GetPosition().Y);
                        break;
                    case SDLK_KP2:
                        if ((Result == Playing) && (!Selector->HasSelection))
                            Selector->SetPosition(Selector->GetPosition().X,Selector->GetPosition().Y+1);
                        break;
                    case SDLK_KP8:
                        if ((Result == Playing) && (!Selector->HasSelection))
                            Selector->SetPosition(Selector->GetPosition().X,Selector->GetPosition().Y-1);
                        break;

                    case SDLK_KP9:
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
                                            if (GlobalSoundEnabled)
                                                Mix_PlayChannel(-1,Sounds[SND_WRONG],0);
                                        }
                                    }
                                    else
                                    {
                                        Bord->ApplyMove(Move,true);
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
                                        MadeMoveList.push_back(Move);
                                        Selector->SetPosition( Selector->GetSelection().X+1,Selector->GetSelection().Y-1);
                                        Selector->HasSelection = false;
                                        Selector->Hide();
                                        PlayerNr = Computer;
                                    }
                                    else
                                    {
                                        if (GlobalSoundEnabled)
                                            Mix_PlayChannel(-1,Sounds[SND_WRONG],0);
                                    }
                                }
                                else
                                {
                                     if (GlobalSoundEnabled)
                                        Mix_PlayChannel(-1,Sounds[SND_WRONG],0);
                                }
                            }

                        }
                        break;
                    case SDLK_KP7:
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
                                            if (GlobalSoundEnabled)
                                                Mix_PlayChannel(-1,Sounds[SND_WRONG],0);
                                        }
                                    }
                                    else
                                    {
                                        Bord->ApplyMove(Move,true);
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
                                        MadeMoveList.push_back(Move);
                                        Selector->SetPosition( Selector->GetSelection().X-1,Selector->GetSelection().Y-1);
                                        Selector->HasSelection = false;
                                        Selector->Hide();
                                        PlayerNr = Computer;
                                    }
                                    else
                                    {
                                        if (GlobalSoundEnabled)
                                            Mix_PlayChannel(-1,Sounds[SND_WRONG],0);
                                    }
                                }
                                else
                                {
                                    if (GlobalSoundEnabled)
                                        Mix_PlayChannel(-1,Sounds[SND_WRONG],0);
                                }
                            }

                        }
                        break;
                    case SDLK_KP1:
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
                                            if (GlobalSoundEnabled)
                                                Mix_PlayChannel(-1,Sounds[SND_WRONG],0);
                                        }
                                    }
                                    else
                                    {
                                        Bord->ApplyMove(Move,true);
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
                                        MadeMoveList.push_back(Move);
                                        Selector->SetPosition( Selector->GetSelection().X-1,Selector->GetSelection().Y+1);
                                        Selector->HasSelection = false;
                                        Selector->Hide();
                                        PlayerNr = Computer;
                                    }
                                    else
                                    {
                                        if (GlobalSoundEnabled)
                                            Mix_PlayChannel(-1,Sounds[SND_WRONG],0);
                                    }
                                }
                                else
                                {
                                    if (GlobalSoundEnabled)
                                        Mix_PlayChannel(-1,Sounds[SND_WRONG],0);
                                }
                            }

                        }
                        break;
                     case SDLK_KP3:
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
                                            if (GlobalSoundEnabled)
                                                Mix_PlayChannel(-1,Sounds[SND_WRONG],0);
                                        }
                                    }
                                    else
                                    {
                                        Bord->ApplyMove(Move,true);
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
                                        MadeMoveList.push_back(Move);
                                        Selector->SetPosition( Selector->GetSelection().X+1,Selector->GetSelection().Y+1);
                                        Selector->HasSelection = false;
                                        Selector->Hide();
                                        PlayerNr = Computer;
                                    }
                                    else
                                    {
                                        if (GlobalSoundEnabled)
                                            Mix_PlayChannel(-1,Sounds[SND_WRONG],0);
                                    }
                                }
                                else
                                {
                                    if (GlobalSoundEnabled)
                                        Mix_PlayChannel(-1,Sounds[SND_WRONG],0);
                                }
                            }

                        }
                        break;
                    case SDLK_a:
                        if (Result == Playing)
                        {
                            if (!Selector->HasSelection && ((Bord->GetBordValue(Selector->GetPosition().X,Selector->GetPosition().Y) == HumanPawn) || (Bord->GetBordValue(Selector->GetPosition().X,Selector->GetPosition().Y) == HumanKing)))
                            {
                                if (GlobalSoundEnabled)
                                    Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                                Selector->Select();
                            }
                            else
                                Selector->DeSelect();
                        }
                        else
                            GameState = GSTitleScreen;
                        break;
					default:
						break;
                }
        }
        Bord->Draw(Screen);
        if (!Bord->GameEnded())
            Selector->Draw(Screen,Bord);
        SDL_BlitSurface(IMGPanel,NULL,Screen,&PanelRect);
        if(PlayerNr == Human)
            SDL_BlitSurface(IMGPlayer,NULL,Screen,&PlayerRect);
        else
            SDL_BlitSurface(IMGCpu,NULL,Screen,&PlayerRect);
        if(Result == Draw)
            SDL_BlitSurface(IMGDraw,NULL,Screen,&ResultPanel);
        if(Result == PlayerWin)
            SDL_BlitSurface(IMGPlayerWins,NULL,Screen,&ResultPanel);
        if(Result == CPUWin)
            SDL_BlitSurface(IMGCpuWins,NULL,Screen,&ResultPanel);


        if((int)(MadeMoveList.size()-8) < 0)
            StartFor = 0;
        else
            StartFor = (Sint16)MadeMoveList.size()-8;
        for(size_t i=StartFor;i<MadeMoveList.size();i++)
        {
            sprintf(Msg,"%d,%d > %d,%d",MadeMoveList[i].X+1,MadeMoveList[i].Y+1,MadeMoveList[i].NewX+1,MadeMoveList[i].NewY+1);
            if(MadeMoveList[i].Player == Human)
            {
				SDL_Rect rect = {247,(Sint16)(80+((i-StartFor)*18)),66, 18};
				SDL_FillRect(Screen,&rect,SDL_MapRGB(Screen->format, 255,255,255));
				WriteText(Screen,font,Msg,strlen(Msg),247,80+((i-StartFor)*18),0,Color2);
			}    
            else
			{
				SDL_Rect rect = {247,(Sint16)(80+((i-StartFor)*18)),66, 18};
				SDL_FillRect(Screen,&rect,SDL_MapRGB(Screen->format, 0,0,0));
                WriteText(Screen,font,Msg,strlen(Msg),247,80+((i-StartFor)*18),0,Color1);
			}

        }
        SDL_Flip(Screen);
        if(PlayerNr==Computer && !Bord->GameEnded())
        {
            if(Ai->MakeMove(Bord,Computer))
            {
                PlayerNr = Human;
                Selector->Show();
            }
            else
            {
                Result = PlayerWin;
                if (GlobalSoundEnabled)
                    Mix_PlayChannel(-1,Sounds[SND_WINNER],0);
            }
        }
        if((Result==Playing) && (PlayerNr == Human) && (Bord->GetTakeMoves(Human).size() == 0) && (Bord->GetNormalMoves(Human).size() == 0))
        {
            Selector->Hide();
            Result = CPUWin;
            if (GlobalSoundEnabled)
                Mix_PlayChannel(-1,Sounds[SND_LOSER],0);
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
					if (GlobalSoundEnabled)
                		Mix_PlayChannel(-1,Sounds[SND_DRAW],0);
                }
                else
                    if(NumHumanPieces > 0)
                    {
                        Result = PlayerWin;
                        if (GlobalSoundEnabled)
                            Mix_PlayChannel(-1,Sounds[SND_WINNER],0);
                    }
                    else
                        if(NumComputerPieces > 0)
                        {
                            Result = CPUWin;
                            if (GlobalSoundEnabled)
                            {
                                Mix_PlayChannel(-1,Sounds[SND_LOSER],0);
                            }
                        }
            }
        }
   }
   UnLoadGameGraphics();
   delete Selector;
   delete Ai;
   delete Bord;
   Mix_HaltChannel(-1);

}