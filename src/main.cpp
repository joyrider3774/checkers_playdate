#include <iostream>
#include <vector>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_Main.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_gfxPrimitives.h>

using namespace std;
const int
        WINDOW_WIDTH = 320,
		WINDOW_HEIGHT = 240,
		NrOfRows = 8,
        NrOfCols = 8,
        ComputerPawn = 3,
        HumanPawn = 1,
        ComputerKing = 4,
        HumanKing = 2,
        Human = 1,
        Computer = 2,
        Empty = 0,
        TileWidth = 30,
        TileHeight = 30,
        VeryEasy=1,
        Easy=2,
        Hard=3,
        VeryHard=4,
        NrOfSounds=8,
        SND_SELECT=0,
		SND_TAKEMOVE=1,
		SND_WRONG=2,
		SND_START=3,
		SND_WINNER=4,
		SND_LOSER=5,
		SND_MENU=6,
		SND_MOVE=7,
        BordValues[8][8] = {{4,2,2,2,2,2,2,4},
                            {4,3,2,5,5,2,3,4},
                            {4,3,2,5,5,2,3,4},
                            {4,3,2,5,5,2,3,4},
                            {4,3,2,5,5,2,3,4},
                            {4,3,2,5,5,2,3,4},
                            {4,3,2,5,5,2,3,4},
                            {4,2,2,2,2,2,2,4}},
        BordValues1[8][8] = {{4,2,2,2,2,2,2,4},
                            {4,3,2,5,5,2,3,4},
                            {4,3,2,5,5,2,3,4},
                            {4,3,2,5,5,2,3,4},
                            {4,3,2,5,5,2,3,4},
                            {4,3,2,5,5,2,3,4},
                            {4,3,2,5,5,2,3,4},
                            {4,2,2,2,2,2,2,4}};

SDL_Surface *Screen, *WhiteSquare,*BlackSquare,*BluePawn,*RedPawn,*RedKing,*BlueKing;
Mix_Chunk *Sounds[NrOfSounds];
enum GameStates {GSQuit,GSGame,GSTitleScreen,GSOptions};
enum GameResult {CPUWin,PlayerWin,Draw,Playing};
int FunctionCalls = 0;
Uint32 Ttime=0;
bool GlobalSoundEnabled=true;

struct SMove
{
    int X,Y,NewX,NewY,DeletedPiece,Player,KingX,KingY;
};

vector<SMove> MadeMoveList;

struct SValueMove
{
    SMove Move;
    int Value;
};



// A struct that holds x & y coordinate
struct SPoint
{
    int X,Y;
};

void PrintMoves1(vector<SMove>  List)
{
    vector<SMove>::iterator iter1=List.begin();
    int teller=0;
    while(iter1 != List.end())
    {
        teller++;
        printf("Move %d (%d,%d) -> (%d,%d)\n",teller,iter1->X,iter1->Y,iter1->NewX,iter1->NewY);
        iter1++;

    }
}

void PrintMoves(vector<vector<SMove> > List)
{
    vector<SMove> iter2;
    size_t x,y;
    printf("%llu\n",List.size());
    for (y=0;y<List.size();y++)
    {
        for (x=0;x<List[y].size();x++)
        {
            printf("Move %llu (%d,%d) -> (%d,%d) KingX:%d KingY:%d\n",y,List[y][x].X,List[y][x].Y,List[y][x].NewX,List[y][x].NewY,List[y][x].KingX,List[y][x].KingY);
        }
    }
}

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

// constructor of main menu will Load the graphics and set the current selection to 1 (io newgame)
CMainMenu::CMainMenu()
{
    Selection = 1;
    SDL_Surface *Tmp;

    Tmp = IMG_Load("./Source/graphics/titlescreen.png");
    IMGTitleScreen = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/play1.png");
    IMGPlay1 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/play2.png");
    IMGPlay2 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/options1.png");
    IMGOptions1 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/options2.png");
    IMGOptions2 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/quit1.png");
    IMGQuit1 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/quit2.png");
    IMGQuit2 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

}

// Destructor will free the surface images
CMainMenu::~CMainMenu()
{
    SDL_FreeSurface(IMGPlay1);
    SDL_FreeSurface(IMGPlay2);
    SDL_FreeSurface(IMGQuit1);
    SDL_FreeSurface(IMGQuit2);
    SDL_FreeSurface(IMGOptions1);
    SDL_FreeSurface(IMGOptions2);
    SDL_FreeSurface(IMGTitleScreen);
}

// Increase the selection if it goes to far set i to the first selection
void CMainMenu::NextItem()
{
    Selection++;
    if (Selection == 4)
        Selection = 1;
    if (GlobalSoundEnabled)
        Mix_PlayChannel(-1,Sounds[SND_MENU],0);
}

// decrease the selection if it goes to low set it to the last selection
void CMainMenu::PreviousItem()
{
    Selection--;
    if (Selection == 0)
        Selection = 3;
    if (GlobalSoundEnabled)
        Mix_PlayChannel(-1,Sounds[SND_MENU],0);
}

// Draw the main menu
void CMainMenu::Draw(SDL_Surface *Surface)
{
    SDL_Rect Dest;

    // draw the title screen background
    SDL_BlitSurface(IMGTitleScreen,NULL,Surface,NULL);


    if (Selection == 1)
    {
        Dest.x = 100;
        Dest.y = 62;
        Dest.w = IMGPlay1->w;
        Dest.h = IMGPlay1->h;
        SDL_BlitSurface(IMGPlay1,NULL,Surface,&Dest);
    }
    else
    {
        Dest.x = 100;
        Dest.y = 62;
        Dest.w = IMGPlay2->w;
        Dest.h = IMGPlay2->h;
        SDL_BlitSurface(IMGPlay2,NULL,Surface,&Dest);
    }

    if (Selection == 2)
    {
        Dest.x = 100;
        Dest.y = 107;
        Dest.w = IMGOptions1->w;
        Dest.h = IMGOptions1->h;
        SDL_BlitSurface(IMGOptions1,NULL,Surface,&Dest);
    }
    else
    {
        Dest.x = 100;
        Dest.y = 107;
        Dest.w = IMGOptions2->w;
        Dest.h = IMGOptions2->h;
        SDL_BlitSurface(IMGOptions2,NULL,Surface,&Dest);
    }


    if (Selection == 3) // = quit draw purple
    {
        Dest.x = 100;
        Dest.y = 150;
        Dest.w = IMGQuit1->w;
        Dest.h = IMGQuit1->h;
        SDL_BlitSurface(IMGQuit1,NULL,Surface,&Dest);
    }
    else // draw blue
    {
        Dest.x = 100;
        Dest.y = 150;
        Dest.w = IMGQuit2->w;
        Dest.h = IMGQuit2->h;
        SDL_BlitSurface(IMGQuit2,NULL,Surface,&Dest);
    }

}


class COptionMenu
{
    private:
        bool OptionJump;
        int Selection,OptionDifficulty;
        SDL_Surface *IMGVeryEasy1,*IMGVeryEasy2,*IMGEasy1,*IMGEasy2,*IMGHard1,*IMGHard2,*IMGVeryHard1,*IMGVeryHard2,*IMGJumpEnabled1,*IMGJumpEnabled2,*IMGJumpDisabled1,*IMGJumpDisabled2,*IMGTitleScreen; // the surfaces needed for the graphics inside the game
    public:
        COptionMenu(int Difficulty,bool JumpEnabled); // constructor
        void Draw(SDL_Surface *Surface); //draw to a certain surface
        void NextItem(); //Select the next menuitem
        void PreviousItem(); //select the previous menuitem
        void NextOption();
        void PreviousOption();
        int GetOptionDifficulty() {return OptionDifficulty;}
        bool GetOptionJump(){return OptionJump;}
        int GetSelection() { return Selection; } // return the current selection
        ~COptionMenu(); // destructor
};

COptionMenu::COptionMenu(int Difficulty,bool JumpEnabled)
{
    Selection = 1;
    OptionDifficulty = Difficulty;
    OptionJump = JumpEnabled;
    SDL_Surface *Tmp;

    Tmp = IMG_Load("./Source/graphics/titlescreen.png");
    IMGTitleScreen = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/veryeasy1.png");
    IMGVeryEasy1 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/veryeasy2.png");
    IMGVeryEasy2 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/easy1.png");
    IMGEasy1 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/easy2.png");
    IMGEasy2 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/hard1.png");
    IMGHard1 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/hard2.png");
    IMGHard2 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/veryhard1.png");
    IMGVeryHard1 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/veryhard2.png");
    IMGVeryHard2 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/jumpenabled1.png");
    IMGJumpEnabled1 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/jumpenabled2.png");
    IMGJumpEnabled2 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/jumpdisabled1.png");
    IMGJumpDisabled1 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

    Tmp = IMG_Load("./Source/graphics/jumpdisabled2.png");
    IMGJumpDisabled2 = SDL_DisplayFormatAlpha(Tmp);
    SDL_FreeSurface(Tmp);

}

// Destructor will free the surface images
COptionMenu::~COptionMenu()
{
    SDL_FreeSurface(IMGTitleScreen);
    SDL_FreeSurface(IMGVeryEasy1);
    SDL_FreeSurface(IMGVeryEasy2);
    SDL_FreeSurface(IMGEasy1);
    SDL_FreeSurface(IMGEasy2);
    SDL_FreeSurface(IMGHard1);
    SDL_FreeSurface(IMGHard2);
    SDL_FreeSurface(IMGVeryHard1);
    SDL_FreeSurface(IMGVeryHard2);
    SDL_FreeSurface(IMGJumpEnabled1);
    SDL_FreeSurface(IMGJumpEnabled2);
    SDL_FreeSurface(IMGJumpDisabled1);
    SDL_FreeSurface(IMGJumpDisabled2);
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
    if (GlobalSoundEnabled)
        Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
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
    if (GlobalSoundEnabled)
        Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
}

void COptionMenu::NextItem()
{
    Selection++;
    if (Selection == 3)
        Selection = 1;
    if (GlobalSoundEnabled)
        Mix_PlayChannel(-1,Sounds[SND_MENU],0);
}

void COptionMenu::PreviousItem()
{
    Selection--;
    if (Selection == 0)
        Selection = 2;
    if (GlobalSoundEnabled)
        Mix_PlayChannel(-1,Sounds[SND_MENU],0);
}

// Draw the main menu
void COptionMenu::Draw(SDL_Surface *Surface)
{
    SDL_Rect Dest;

    // draw the title screen background
    SDL_BlitSurface(IMGTitleScreen,NULL,Surface,NULL);


    if (Selection == 1)
    {
        Dest.x = 40;
        Dest.y = 62;
        if(OptionJump)
        {
            Dest.w = IMGJumpEnabled1->w;
            Dest.h = IMGJumpEnabled1->h;
            SDL_BlitSurface(IMGJumpEnabled1,NULL,Surface,&Dest);
        }
        else
        {
            Dest.w = IMGJumpDisabled1->w;
            Dest.h = IMGJumpDisabled1->h;
            SDL_BlitSurface(IMGJumpDisabled1,NULL,Surface,&Dest);
        }
    }
    else
    {
        Dest.x = 40;
        Dest.y = 62;
        if(OptionJump)
        {
            Dest.w = IMGJumpEnabled2->w;
            Dest.h = IMGJumpEnabled2->h;
            SDL_BlitSurface(IMGJumpEnabled2,NULL,Surface,&Dest);
        }
        else
        {
            Dest.w = IMGJumpDisabled2->w;
            Dest.h = IMGJumpDisabled2->h;
            SDL_BlitSurface(IMGJumpDisabled2,NULL,Surface,&Dest);
        }
    }

    if (Selection == 2)
    {
        Dest.x = 40;
        Dest.y = 119;
        switch(OptionDifficulty)
        {
            case VeryEasy:
                Dest.w = IMGVeryEasy1->w;
                Dest.h = IMGVeryEasy1->h;
                SDL_BlitSurface(IMGVeryEasy1,NULL,Surface,&Dest);
                break;
            case Easy:
                Dest.w = IMGEasy1->w;
                Dest.h = IMGEasy1->h;
                SDL_BlitSurface(IMGEasy1,NULL,Surface,&Dest);
                break;
            case Hard:
                Dest.w = IMGHard1->w;
                Dest.h = IMGHard1->h;
                SDL_BlitSurface(IMGHard1,NULL,Surface,&Dest);
                break;
            case VeryHard:
                Dest.w = IMGVeryHard1->w;
                Dest.h = IMGVeryHard1->h;
                SDL_BlitSurface(IMGVeryHard1,NULL,Surface,&Dest);
                break;

        }

    }
    else
    {
        Dest.x = 40;
        Dest.y = 119;
        switch(OptionDifficulty)
        {
            case VeryEasy:
                Dest.w = IMGVeryEasy2->w;
                Dest.h = IMGVeryEasy2->h;
                SDL_BlitSurface(IMGVeryEasy2,NULL,Surface,&Dest);
                break;
            case Easy:
                Dest.w = IMGEasy2->w;
                Dest.h = IMGEasy2->h;
                SDL_BlitSurface(IMGEasy2,NULL,Surface,&Dest);
                break;
            case Hard:
                Dest.w = IMGHard2->w;
                Dest.h = IMGHard2->h;
                SDL_BlitSurface(IMGHard2,NULL,Surface,&Dest);
                break;
            case VeryHard:
                Dest.w = IMGVeryHard2->w;
                Dest.h = IMGVeryHard2->h;
                SDL_BlitSurface(IMGVeryHard2,NULL,Surface,&Dest);
                break;

        }
    }

}


class CBord
{
    private:
        int BordArray[8][8];
    public:
        CBord();
        CBord(CBord *Bord);
        void ApplyMove(SMove &move,bool PlaySound);
        void ApplyMoves(vector<SMove> &Moves,bool reverse,bool DelayDraw,bool PlaySound);
        void UndoMove(SMove &Move);
        void UndoMoves(vector<SMove> &Moves,bool reverse);
        void Draw(SDL_Surface *Surface);
        bool ValidMove(SMove move);
        int GetBordValue(int X,int Y);
        bool GameEnded();
        bool PieceCanJump(int X,int Y);
        bool PieceCanMove(int X,int Y);
        vector<SMove> GetSingleTakeMoves(int Player);
        vector<SMove> GetSingleTakeMoves(int Player,int X,int Y);
        vector<vector<SMove> > GetTakeMoves(int Player);
        vector<vector<SMove> > GenerateTakeMoves(int Player,int X,int Y);
        vector<vector<SMove> > GetNormalMoves(int Player);
};

class CSelector
{
    private:
        SPoint SelectedPoint,CurrentPoint; //variables to hold the current position and the selected position
		SDL_Surface *IMGSelect, *IMGSelectMove;
    public:
        bool HasSelection,Visible;
        // constructor, parameters define the starting position
        CSelector(const int PlayFieldXin,const int PlayFieldYin)
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
        void Hide()
        {
            Visible = false;
        }
        void Show()
        {
            Visible = true;
        }
        void SetPosition(const int PlayFieldXin,const int PlayFieldYin)
        {
            // check if the new position is inside the board, if so set the new position as the currentposition
            if ((PlayFieldYin>=0) && (PlayFieldYin < NrOfRows) && (PlayFieldXin >= 0) && (PlayFieldXin < NrOfCols))
            {
                CurrentPoint.X = PlayFieldXin;
                CurrentPoint.Y = PlayFieldYin;
            }
        }
        // Select will set the current position as the selected position
        void Select()
        {
            SelectedPoint.X = CurrentPoint.X;
            SelectedPoint.Y = CurrentPoint.Y;
            HasSelection = true;
        }
        // return the selected position
        SPoint GetSelection()
        {
            return SelectedPoint;
        }
        // return the current position
        SPoint GetPosition()
        {
            return CurrentPoint;
        }
        void DeSelect()
        {
            HasSelection = false;
        }
        // Draw the blue box on the current position, with the offsets in mind
        void Draw(SDL_Surface *Surface,CBord *Bord)
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
        ~CSelector()
        {
			SDL_FreeSurface(IMGSelect);
    		SDL_FreeSurface(IMGSelectMove);
        }
};

CSelector *Selector;

class ComputerAi
{   
	protected:
        int SearchDepth;
        bool JumpHeuristicEnabled;
        int EndGame(CBord *Bord,int Player);
        float Distance(int X1,int Y1,int X2,int Y2);
    private:
        int Max(CBord *Bord,int depth,int alpha,int beta);
        int Min(CBord *Bord,int depth,int alpha,int beta);
        vector<SMove> MinMax(CBord *Bord,int depth,int Player);
        virtual int EvaluateBord(CBord *Bord,int Player);
        vector<vector<SMove> > GenerateMovesList(CBord *Bord,int Player);
    public:
        ComputerAi(int SearchDepthIn,bool JumpHeuristicEnabledIn);
        bool MakeMove(CBord *Bord,int Player);
        virtual ~ComputerAi();
};


ComputerAi::ComputerAi(int SearchDepthIn,bool JumpHeuristicEnabledIn)
{
    SearchDepth=SearchDepthIn;
    JumpHeuristicEnabled = JumpHeuristicEnabledIn;
}

ComputerAi::~ComputerAi()
{
}

float ComputerAi::Distance(int X1,int Y1,int X2,int Y2)
{
    int X_Dist, Y_Dist;

    X_Dist = (X1 - X2);
    X_Dist *= X_Dist;
    Y_Dist = (Y1 - Y2);
    Y_Dist *= Y_Dist;
    if (X_Dist + Y_Dist > 0)
        return sqrt(X_Dist + Y_Dist);
    else
        return 0;
}

int ComputerAi::EndGame(CBord *Bord,int Player)
{
    int X1,Y1,X2,Y2,NumPieces=0;
    int PieceToFind1,PieceToFind2,OppPiece1,OppPiece2;
    float Dist,MaxDist=0;

    if(Player == Human)
    {
        PieceToFind1 = HumanKing;
        PieceToFind2 = HumanKing;
        OppPiece1 = ComputerKing;
        OppPiece2 = ComputerPawn;
    }
    else
    {
        PieceToFind1 = ComputerKing ;
        PieceToFind2 = ComputerKing;
        OppPiece1 = HumanKing;
        OppPiece2 = HumanPawn;
    }
    Dist = 0;
    for (Y1 = 0;Y1 < NrOfRows;Y1++)
        for (X1 = 0; X1 < NrOfCols; X1++)
        {
            if((Bord->GetBordValue(X1,Y1) == PieceToFind1) ||(Bord->GetBordValue(X1,Y1) == PieceToFind2))
            {

                for (Y2 = 0;Y2 < NrOfRows;Y2++)
                    for (X2= 0; X2 < NrOfCols; X2++)
                    {
                        if((Bord->GetBordValue(X2,Y2) == OppPiece1) ||(Bord->GetBordValue(X2,Y2) == OppPiece2))
                        {
                            NumPieces++;
                            Dist += Distance(X1,Y1,X2,Y2);
                            if (Dist > MaxDist)
                                MaxDist = Dist;
                        }
                    }
            }
        }
  if (NumPieces == 0)
    return 0;
  else
    return (int) ((Dist/NumPieces));
}
vector<vector<SMove> > ComputerAi::GenerateMovesList(CBord *Bord,int Player)
{
    vector<vector<SMove> > MoveList,MoveList1;
    MoveList = Bord->GetTakeMoves(Player);
    if(MoveList.size() > 0)
        return MoveList;
    else
        return Bord->GetNormalMoves(Player);
}

int ComputerAi::EvaluateBord(CBord *Bord,int Player)
{

    vector<vector<SMove> > TakeMoves;
    int NumHumanJumps=0,NumHumanMoves=0,NumComputerJumps=0,NumComputerMoves=0,Value=0,X,Y,NumComputerPawns=0,NumHumanPawns=0,NumComputerKings=0,NumHumanKings=0;
    for (Y=0;Y<NrOfRows;Y++)
        for (X=0;X<NrOfCols;X++)
        {
            if ((Y*(NrOfRows-1) + X)%2 == 1)
            {
                if(Bord->GetBordValue(X,Y) == HumanPawn)
                {
                    NumHumanPawns++;
                    if(JumpHeuristicEnabled)
					{
                        if(Bord->PieceCanJump(X,Y))
                            NumHumanJumps++;
                        else
                            if(Bord->PieceCanMove(X,Y))
                                NumHumanMoves++;
					}
					Value -= 7-Y;
                }
                if (Bord->GetBordValue(X,Y) == HumanKing)
                {
                    NumHumanKings++;
                    if(JumpHeuristicEnabled)
					{
                        if(Bord->PieceCanJump(X,Y))
                            NumHumanJumps++;
                        else
                            if(Bord->PieceCanMove(X,Y))
                                NumHumanMoves++;
					}
                    if((Y >= 3) && (Y <= 4))
                    {
                        if((X==0) || (X==1) || (X==6) || (X==7))
                            Value -= 3;
                        else
                            Value -=4;
                    }
                    else
                        if(Y <3)
                        {
                            Value -= Y;
                        }
                        else
                            Value -= (7-Y);


                }
                if(Bord->GetBordValue(X,Y) == ComputerPawn)
                {
                    NumComputerPawns++;
                    if(JumpHeuristicEnabled)
					{
                        if(Bord->PieceCanJump(X,Y))
                            NumComputerJumps++;
                        else
                            if(Bord->PieceCanMove(X,Y))
                                NumComputerMoves++;
					}
                    Value += Y;

                }
                if (Bord->GetBordValue(X,Y) == ComputerKing)
                {
                    NumComputerKings++;
                    if(JumpHeuristicEnabled)
					{
                        if(Bord->PieceCanJump(X,Y))
                            NumComputerJumps++;
                        else
                            if(Bord->PieceCanMove(X,Y))
                                NumComputerMoves++;
					}
                    if((Y >= 3) && (Y <= 4))
                    {
                        if((X==0) || (X==1) || (X==6) || (X==7))
                            Value += 3;
                        else
                            Value +=4;
                    }
                    else
                        if(Y <3)
                        {
                            Value += Y;
                        }
                        else
                            Value += (7-Y);
                }
            }

        }

       Value += ((NumComputerPawns - NumHumanPawns)*40) +((NumComputerKings - NumHumanKings)*80);
       if (JumpHeuristicEnabled)
        {
            if (NumHumanJumps > 0)
                Value -= NumHumanJumps*3;
            else
                Value -= NumHumanMoves;
            if (NumComputerJumps > 0)
                Value += NumComputerJumps*3;
            else
                Value += NumComputerMoves;
        }

    return  Value;
}

int ComputerAi::Max(CBord *Bord,int depth, int alpha, int beta)
{
    int Value;
    vector<vector<SMove> >Moves;
    if ((depth <= 0) || Bord->GameEnded())
    {
        Value = EvaluateBord(Bord,Computer);
        return  Value;
    }
    else
    {
        Moves = GenerateMovesList(Bord,Computer);
        if (Moves.size()  == 0)
            return EvaluateBord(Bord,Computer);
        vector<vector<SMove> >::iterator Iter=Moves.begin();
        while (Iter != Moves.end())
        {
            Bord->ApplyMoves(*Iter,true,false,false);
            Value = Min(Bord,depth-1,alpha,beta);
            Bord->UndoMoves(*Iter,false);

            if (Value > alpha)
            {
                alpha = Value;
            }

            if (alpha > beta)
            {
                vector<vector<SMove> >().swap(Moves);
                return beta;
            }
            Iter++;

        }
        vector<vector<SMove> >().swap(Moves);
        return alpha;
    }
}

int ComputerAi::Min(CBord *Bord,int depth, int alpha, int beta)
{
    int Value;
    vector<vector<SMove> > Moves;
    if ((depth <= 0) || Bord->GameEnded() )
    {
        Value = EvaluateBord(Bord,Human);
        return  Value;

    }
    else
    {
        Moves = GenerateMovesList(Bord,Human);
        if (Moves.size()  == 0)
        {
            return EvaluateBord(Bord,Human);
            vector<vector<SMove> >().swap(Moves);
        }
        vector<vector<SMove> >::iterator Iter=Moves.begin();
        while (Iter != Moves.end())
        {
            Bord->ApplyMoves(*Iter,true,false,false);
            Value = Max(Bord,depth-1,alpha,beta);
            Bord->UndoMoves(*Iter,false);

            if (Value < beta)
            {
                beta = Value;
            }

            if (beta < alpha)
            {
                vector<vector<SMove> >().swap(Moves);
                return alpha;
            }
            Iter++;
        }
        vector<vector<SMove> >().swap(Moves);
        return beta;
    }

}

vector<SMove> ComputerAi::MinMax(CBord *Bord,int depth,int Player)
{
    FunctionCalls = 0;
    vector<vector<SMove> >Moves;
    int BestValue,Value;
    vector<SMove> BestMove;
    Moves = GenerateMovesList(Bord,Player);
    if(Player == Human)
        PrintMoves(Moves);
    vector<vector<SMove> >::iterator iter = Moves.begin();
    if(Player == Computer)
        BestValue = INT_MIN;
    else
        BestValue = INT_MAX;
    while (iter != Moves.end())
    {
        Bord->ApplyMoves(*iter,true,false,false);

        if (Player==Computer)
        {
            Value = Min(Bord,depth,INT_MIN,INT_MAX);
            Bord->UndoMoves(*iter,false);
            if(Value > BestValue)
            {

                BestMove = *iter;
                BestValue = Value;

            }
        }
        else
        {
            Value = Max(Bord,depth,INT_MIN,INT_MAX);
            Bord->UndoMoves(*iter,false);
            if(Value < BestValue)
            {
                BestMove = *iter;
                BestValue = Value;
            }
        }

        iter++;
    }
    vector<vector<SMove> >().swap(Moves);
    return BestMove;
}

bool  ComputerAi::MakeMove(CBord *Bord,int Player)
{
    Uint32 Start= SDL_GetTicks(),End=0;
    vector<SMove> ComputerMove = MinMax(Bord,SearchDepth,Player);
    size_t Teller;
    End = SDL_GetTicks();
    if(End-Start < 350)
        SDL_Delay(350 - (End-Start));
    //printf("Time Taken to calculate move: %d calls %d\n",SDL_GetTicks()-Start,FunctionCalls);
    Bord->ApplyMoves(ComputerMove,true,true,true);
    for (Teller=0;Teller<ComputerMove.size();Teller++)
        MadeMoveList.push_back(ComputerMove[Teller]);
    return (ComputerMove.size() > 0);

}

CBord::CBord()
{
    int X,Y;
    for (Y=0;Y<NrOfRows;Y++)
        for (X=0;X<NrOfCols;X++)
            if((Y*(NrOfRows-1) + X)&1)
            {
                if (Y<3)
                {
                    BordArray[X][Y] = ComputerPawn;
                }
                else
                    if(Y>4)
                        BordArray[X][Y]=HumanPawn;
                    else
                        BordArray[X][Y] = Empty;
            }
            else
                BordArray[X][Y] = 0;
}

CBord::CBord( CBord *Bord)
{
   int X,Y;
    for (Y=0;Y<NrOfRows;Y++)
        for (X=0;X<NrOfCols;X++)
            BordArray[X][Y] = Bord->GetBordValue(X,Y);
}

bool CBord::PieceCanJump(int X,int Y)
{
    int Value;
    SMove Move;
    Value = BordArray[X][Y];
    if( Value != Empty)
    {
        if((Value == HumanKing) || (Value==HumanPawn))
            Move.Player = Human;
        else
           Move.Player = Computer;

        Move.DeletedPiece = -1;
        Move.KingX = -1;
        Move.KingY = -1;
        Move.X = X;
        Move.Y = Y;
        Move.NewX = X - 2;
        Move.NewY = Y + 2;
        if (ValidMove(Move))
        {
            return true;
        }
        Move.NewY = Y + 2;
        Move.NewX = X + 2;
        if (ValidMove(Move))
        {
            return true;
        }
        Move.NewX = X + 2;
        Move.NewY = Y - 2;
        if (ValidMove(Move))
        {
            return true;
        }
        Move.NewY = Y - 2;
        Move.NewX = X -2;
        if (ValidMove(Move))
        {
            return true;
        }

        return false;
    }
    else
        return false;
}

bool CBord::PieceCanMove(int X,int Y)
{
    int Value;
    SMove Move;
    Value = BordArray[X][Y];
    if( Value != Empty)
    {
        if((Value == HumanKing) || (Value==HumanPawn))
            Move.Player = Human;
        else
           Move.Player = Computer;

        Move.DeletedPiece = -1;
        Move.KingX = -1;
        Move.KingY = -1;
        Move.X = X;
        Move.Y = Y;
        Move.NewX = X - 1;
        Move.NewY = Y + 1;
        if (ValidMove(Move))
        {
            return true;
        }
        Move.NewY = Y + 1;
        Move.NewX = X + 1;
        if (ValidMove(Move))
        {
            return true;
        }
        Move.NewX = X + 1;
        Move.NewY = Y - 1;
        if (ValidMove(Move))
        {
            return true;
        }
        Move.NewY = Y - 1;
        Move.NewX = X -1;
        if (ValidMove(Move))
        {
            return true;
        }

        return false;
    }
    else
        return false;
}


void CBord::ApplyMove(SMove &move,bool PlaySound)
{
    int Xi=0,Yi=0;
    if((move.Player == Computer) && (move.NewY == NrOfRows -1) && (BordArray[move.X][move.Y] == ComputerPawn) &&(move.KingY == -1) && (move.KingX == -1) )
    {
        BordArray[move.NewX][move.NewY] = ComputerKing;
        move.KingY = move.NewY;
        move.KingX = move.NewX;
    }
    else
        if((move.Player == Human) && (move.NewY == 0) && (BordArray[move.X][move.Y] == HumanPawn) && (move.KingY == -1) && (move.KingX == -1))
        {
            BordArray[move.NewX][move.NewY] = HumanKing;
            move.KingY = move.NewY;
            move.KingX = move.NewX;
        }
        else
            BordArray[move.NewX][move.NewY] = BordArray[move.X][move.Y];
    BordArray[move.X][move.Y] = 0;
    Xi = move.NewX - move.X;
    Yi = move.NewY - move.Y;
    if ((abs(Yi) == 2) && (abs(Xi) == 2))
    {
        move.DeletedPiece = BordArray[move.NewX - Xi/2][move.NewY - Yi/2];
        BordArray[move.NewX - Xi/2][move.NewY - Yi/2] = 0;
        if (GlobalSoundEnabled && PlaySound)
            Mix_PlayChannel(-1,Sounds[SND_TAKEMOVE],0);
    }
    else
    {
        if (GlobalSoundEnabled && PlaySound)
            Mix_PlayChannel(-1,Sounds[SND_MOVE],0);
    }
}

void CBord::ApplyMoves(vector<SMove> &Moves,bool reverse,bool DelayDraw,bool PlaySound)
{
    vector<SMove>::iterator iter;
    vector<SMove>::reverse_iterator riter;
    if (reverse)
    {
        riter  = Moves.rbegin();
        while (riter != Moves.rend())
        {
            ApplyMove(*riter,PlaySound);
            riter++;
            if(DelayDraw && (riter != Moves.rend()))
            {
                Draw(Screen);
                SDL_Flip(Screen);
                SDL_Delay(500);
            }
        }
    }
    else
    {
        iter  = Moves.begin();
        while (iter != Moves.end())
        {
            ApplyMove(*iter,PlaySound);
            iter++;
            if(DelayDraw && (riter != Moves.rend()))
            {
                Draw(Screen);
                SDL_Flip(Screen);
                SDL_Delay(500);
            }
        }
    }
}
void CBord::UndoMove(SMove &Move)
{
    int Xi=0,Yi=0;
	if((Move.Player == Computer) && (Move.KingY == Move.NewY) && (Move.KingX == Move.NewX))
    {
        BordArray[Move.X][Move.Y] = ComputerPawn;

        Move.KingX = -1;
        Move.KingY = -1;
    }
    else
        if((Move.Player == Human) && (Move.KingY == Move.NewY) && (Move.KingX == Move.NewX))
        {
            BordArray[Move.X][Move.Y] = HumanPawn;
            Move.KingX = -1;
            Move.KingY = -1;
        }
        else
            BordArray[Move.X][Move.Y] = BordArray[Move.NewX][Move.NewY];
    BordArray[Move.NewX][Move.NewY] = 0;
    Xi = Move.NewX - Move.X;
    Yi = Move.NewY - Move.Y;
    if ((abs(Yi) == 2) && (abs(Xi) == 2))
        BordArray[Move.NewX - Xi/2][Move.NewY - Yi/2] = Move.DeletedPiece;
}

void CBord::UndoMoves(vector<SMove> &Moves,bool reverse)
{
    vector<SMove>::iterator iter;
    vector<SMove>::reverse_iterator riter;
    if (reverse)
    {
        riter  = Moves.rbegin();
        while (riter != Moves.rend())
        {
            printf("Undo Move: (%d,%d) -> (%d,%d)\n",riter->X,riter->Y,riter->NewX,riter->NewY);
            UndoMove(*riter);
            printf("\n");
            riter++;
        }
    }
    else
    {
        iter  = Moves.begin();
        while (iter != Moves.end())
        {
            UndoMove(*iter);
            iter++;
        }
    }

}

void CBord::Draw(SDL_Surface *Surface)
{
    int X,Y;
    SDL_Rect Rect;
    for (Y=0;Y<NrOfRows;Y++)
    {
        for (X=0;X<NrOfCols;X++)
        {
            Rect.x = X * TileWidth;
            Rect.y = Y * TileHeight;
            Rect.w = TileWidth;
            Rect.h = TileHeight;
            switch(BordArray[X][Y])
            {
                case HumanPawn:
                    SDL_BlitSurface(BluePawn,NULL,Surface,&Rect);
                    break;
                case ComputerPawn:
                    SDL_BlitSurface(RedPawn,NULL,Surface,&Rect);
                    break;
                case ComputerKing:
                    SDL_BlitSurface(RedKing,NULL,Surface,&Rect);
                    break;
                case HumanKing:
                    SDL_BlitSurface(BlueKing,NULL,Surface,&Rect);
                    break;
                case 0:
                    if(((Y*(NrOfRows-1))+X) % 2 == 1)
                        SDL_BlitSurface(BlackSquare,NULL,Surface,&Rect);
                    else
                        SDL_BlitSurface(WhiteSquare,NULL,Surface,&Rect);
                    break;
            }
        }
    }
}

int CBord::GetBordValue(int X,int Y)
{
    return BordArray[X][Y];
}

bool CBord::GameEnded()
{
    int X,Y,NumHumanPieces=0,NumComputerPieces=0;
    for (Y=0;Y<NrOfRows;Y++)
        for (X=0;X<NrOfCols;X++)
        {
            if((BordArray[X][Y] == HumanPawn) || (BordArray[X][Y] == HumanKing))
                NumHumanPieces++;
            if((BordArray[X][Y] == ComputerPawn) || (BordArray[X][Y] == ComputerKing))
                NumComputerPieces++;
        }
    return ((NumComputerPieces ==0) || (NumHumanPieces == 0) || ((NumHumanPieces == 1) && (NumComputerPieces == 1) && (GetSingleTakeMoves(Human).size() == 0) && (GetSingleTakeMoves(Computer).size() == 0)));
}

vector<SMove> CBord::GetSingleTakeMoves(int Player,int X,int Y)
{
    SMove Move;
    vector<SMove> TakeMoveList;
    Move.Player = Player;
    Move.DeletedPiece = -1;
    Move.KingX = -1;
    Move.KingY = -1;
    Move.X = X;
    Move.Y = Y;
    Move.NewX = X - 2;
    Move.NewY = Y + 2;
    if (ValidMove(Move))
    {
        TakeMoveList.push_back(Move);
    }
    Move.NewY = Y + 2;
    Move.NewX = X + 2;
    if (ValidMove(Move))
    {
        TakeMoveList.push_back(Move);
    }
    Move.NewX = X + 2;
    Move.NewY = Y - 2;
    if (ValidMove(Move))
    {
        TakeMoveList.push_back(Move);
    }
    Move.NewY = Y - 2;
    Move.NewX = X -2;
    if (ValidMove(Move))
    {
        TakeMoveList.push_back(Move);
    }

    return TakeMoveList;
}


vector<SMove> CBord::GetSingleTakeMoves(int Player)
{
    int X,Y;

    SMove Move;
    vector<SMove> TakeMoveList;
    Move.Player = Player;
    Move.DeletedPiece = -1;
    Move.KingX = -1;
    Move.KingY = -1;
    for (Y=0;Y<NrOfRows;Y++)
        for (X=0;X<NrOfCols;X++)
        {
            if(((Player==Human) &&((BordArray[X][Y] == HumanKing) || (BordArray[X][Y] == HumanPawn))) ||
                ((Player==Computer)&&((BordArray[X][Y] == ComputerKing) || (BordArray[X][Y] == ComputerPawn))))
            {
                Move.X = X;
                Move.Y = Y;
                Move.NewX = X - 2;
                Move.NewY = Y + 2;
                if (ValidMove(Move))
                {
                    TakeMoveList.push_back(Move);
                }
                Move.NewY = Y + 2;
                Move.NewX = X + 2;
                if (ValidMove(Move))
                {
                    TakeMoveList.push_back(Move);
                }
                Move.NewX = X + 2;
                Move.NewY = Y - 2;
                if (ValidMove(Move))
                {
                    TakeMoveList.push_back(Move);
                }
                Move.NewY = Y - 2;
                Move.NewX = X -2;
                if (ValidMove(Move))
                {
                    TakeMoveList.push_back(Move);
                }
            }
        }
    return TakeMoveList;
}

vector<vector<SMove> > CBord::GetNormalMoves(int Player)
{
    int X,Y;
    SMove Move;
    vector<vector<SMove> > MoveList;
    vector<SMove> Moves;
    Move.Player = Player;
    Move.DeletedPiece = -1;
    Move.KingX = -1;
    Move.KingY = -1;
    for (Y=0;Y<NrOfRows;Y++)
        for (X=0;X<NrOfCols;X++)
        {
            if(((Player==Human) &&((BordArray[X][Y] == HumanKing) || (BordArray[X][Y] == HumanPawn))) ||
                ((Player==Computer)&&((BordArray[X][Y] == ComputerKing) || (BordArray[X][Y] == ComputerPawn))))
            {
                Move.X = X;
                Move.Y = Y;
                Move.NewX = X - 1;
                Move.NewY = Y + 1;
                if (ValidMove(Move))
                {
                    Moves.push_back(Move);
                    MoveList.push_back(Moves);
                    Moves.clear();
                }
                Move.NewX = X + 1;
                if (ValidMove(Move))
                {
                    Moves.push_back(Move);
                    MoveList.push_back(Moves);
                    Moves.clear();
                }
                Move.NewY = Y - 1;
                if (ValidMove(Move))
                {
                    Moves.push_back(Move);
                    MoveList.push_back(Moves);
                    Moves.clear();
                }
                Move.NewX = X -1;
                if (ValidMove(Move))
                {
                    Moves.push_back(Move);
                    MoveList.push_back(Moves);
                    Moves.clear();
                }
            }
        }
    return MoveList;
}

vector<vector<SMove> > CBord::GetTakeMoves(int Player)
{
    int X,Y;
	size_t Teller;
    vector<vector<SMove> > TakeMoveList,TakeMoveList1;
    for (Y=0;Y<NrOfRows;Y++)
        for (X=0;X<NrOfCols;X++)
        {
            if(((Player==Human) &&((BordArray[X][Y] == HumanKing) || (BordArray[X][Y] == HumanPawn))) ||
                ((Player==Computer)&&((BordArray[X][Y] == ComputerKing) || (BordArray[X][Y] == ComputerPawn))))
            {
                TakeMoveList1 = GenerateTakeMoves(Player,X,Y);
                if(TakeMoveList1.size() > 0)
				{
                    if(TakeMoveList.size() == 0)
                        TakeMoveList = TakeMoveList1;
                    else
                    {
                        for(Teller=0;Teller<TakeMoveList1.size();Teller++)
                        {
                            TakeMoveList.push_back(TakeMoveList1[Teller]);
                        }
                    }
				}
            }
        }
    return TakeMoveList;
}

vector<vector<SMove> > CBord::GenerateTakeMoves(int Player,int X,int Y)
{
    SMove Move;
    int NumMoves=0;
    vector<SMove> Moves,Moves1;
    vector<vector<SMove> > MoveList,MoveList1;
    vector<SMove>::iterator iter;
    vector<vector<SMove> >::iterator iter2;
    Move.Player = Player;
    size_t teller1,teller2,MoveListIntialSize;
    bool found;
    //check for must take moves
    Move.X = X;
    Move.Y = Y;    
    Move.DeletedPiece = -1;
    Move.KingX = -1;
    Move.KingY = -1;
	SPoint MoveAdditions[] = {{-2, 2}, {2, 2}, {2, -2}, {-2, -2}};
	for (size_t i = 0; i < sizeof(MoveAdditions) / sizeof(MoveAdditions[0]); i++)
	{
		Move.NewX = X + MoveAdditions[i].X;
    	Move.NewY = Y + MoveAdditions[i].Y;
     
		if (ValidMove(Move))
		{		
			Moves.push_back(Move);
			ApplyMove(Move,false);
			//als stuk niet gekroond geweest is
			if((Move.KingX == -1) && (Move.KingY==-1))
			{
				MoveList1 = GenerateTakeMoves(Player,Move.NewX,Move.NewY);
				// als er nog geen moves waren, kopieer gewoon de inhoud (gebeurt hier altijd normaal, is eerste move die we checken)
				if(NumMoves ==0)
					MoveList.assign(MoveList1.begin(),MoveList1.end());
				else
				{
					// anders voeg de al gevonden moves toe aan de movelist
					iter2 = MoveList1.begin();
					while(iter2 != MoveList1.end())
					{
						MoveList.push_back(*iter2);
						iter2++;
					}
				}
			}
			UndoMove(Move);
			NumMoves++;
		}
	}

    //Als er moves gevonden waren
    if(NumMoves > 0)
    {
        //en er zijn geen vorige gevonden items, voeg de gevonden moves als het eerste stuk van een nieuwe move toe
        if(MoveList.size() == 0)
        {
            iter = Moves.begin();
            while(iter != Moves.end())
            {
                Moves1.clear();
                Moves1.push_back(*iter);
                MoveList.push_back(Moves1);
                iter++;
            }
        }
        //anders kopieer de gevonden moves bij de huidige movelist en vul de stappen aan.
        else
        {
            MoveListIntialSize = MoveList.size();
            for(teller1=0;teller1<Moves.size();teller1++)
            {
                found = false;
                for(teller2=0;teller2<MoveListIntialSize;teller2++)
                {

                    if(((MoveList[teller2].back()).X == Moves[teller1].NewX) && ((MoveList[teller2].back()).Y == Moves[teller1].NewY))
                    {
                        MoveList[teller2].push_back(Moves[teller1]);
                        found = true;
                    }

                }
                //indien we de vorige coordinaten niet gevonden hebben is het een eindpunt van een
                //andere move, voeg deze dus toe aan de lijst als een nieuwe move
                if (!found)
                {
                    Moves1.clear();
                    Moves1.push_back(Moves[teller1]);
                    MoveList.push_back(Moves1);
                }
            }
        }

    }
    return MoveList;
}


bool  CBord::ValidMove(SMove move)
{
    int Xi=0,Yi=0,Player=move.Player;
    if ((move.NewX >=0 ) && (move.NewX <NrOfCols) && (move.NewY >=0) && (move.NewY < NrOfRows))
    {
        if (((move.X + (move.Y * (NrOfRows-1))) & 1) && ((move.NewX + (move.NewY * (NrOfRows-1))) & 1))
        {
            // Nieuwe Plek is leeg
            if (BordArray[move.NewX][move.NewY] == 0)
            {
                // Huidige plek is niet leeg
                if (BordArray[move.X][move.Y] == Empty)
                {
                    return false;

                }
                else
                {

                    Xi = move.NewX - move.X;
                    Yi = move.NewY - move.Y;
                    //Human Voorwaarts enkele stap
                    if((BordArray[move.X][move.Y] == HumanPawn) || (BordArray[move.X][move.Y] == HumanKing))
                    {
                        if ((Player == Human) && (Yi == -1) && (abs(Xi) == 1))
                        {
                            return true;
                        }
                        else
						{
							// Human voorwaarts pakken
                            if ((Player == Human) && (Yi == -2) && (abs(Xi) == 2))
                            {
                                if((BordArray[move.NewX - Xi/2][move.NewY - Yi/2] == ComputerPawn) || (BordArray[move.NewX - Xi/2][move.NewY - Yi/2] == ComputerKing))
                                    return true;
                                else
                                    return false;
                            }
                            else
							{
								//human achterwaards enkele stap
                                if ((Player == Human) && (Yi == 1) && (abs(Xi) == 1))
                                {
                                    return (BordArray[move.X][move.Y] == HumanKing);
                                }
                                else
								{
									//human achterwaards pakken
                                    if ((Player == Human) && (Yi == 2) && (abs(Xi) == 2))
                                    {
                                        if((BordArray[move.NewX - Xi/2][move.NewY - Yi/2] == ComputerPawn) || (BordArray[move.NewX - Xi/2][move.NewY - Yi/2] == ComputerKing))
                                        {
                                            if((BordArray[move.X][move.Y] == HumanKing))
                                                return true;
                                            else
                                                return false;
                                        }
                                        else
										{
                                            return false;
										}

                                    }
                                    else
									{
                                        return false;
									}
								}
							}
						}
                    }
                    else
					{
                        if ((BordArray[move.X][move.Y] == ComputerPawn) || (BordArray[move.X][move.Y] == ComputerKing))
                        {   //computer voorwaartse stap
                            if ((Player == Computer) && (Yi == 1) && (abs(Xi) == 1))
                            {
                                return true;
                            }
                            else// Computer voorwaarts pakken
                            {

								if ((Player == Computer) && (Yi == 2) && (abs(Xi) == 2))
                                {
                                    if((BordArray[move.NewX - Xi/2][move.NewY - Yi/2] == HumanPawn) || (BordArray[move.NewX - Xi/2][move.NewY - Yi/2] == HumanKing))
                                        return true;
                                    else
                                        return false;
                                }
                                else
								{ 	
									//Computer achterwaards enkele stap
                                    if ((Player == Computer) && (Yi == -1) && (abs(Xi) == 1))
                                    {
                                        return (BordArray[move.X][move.Y] == ComputerKing);
                                    }
                                    else	//human achterwaards pakken
								    {
										if ((Player == Computer) && (Yi == -2) && (abs(Xi) == 2))
                                        {
                                            if((BordArray[move.NewX - Xi/2][move.NewY - Yi/2] == HumanPawn) || (BordArray[move.NewX - Xi/2][move.NewY - Yi/2] == HumanKing))
                                            {
                                                if (BordArray[move.X][move.Y] == ComputerKing)
                                                    return true;
                                                else
                                                    return false;
                                            }
                                            else
											{
                                                return false ;
											}
                                        }
                                        else
										{
                                            return false;
										}
									}
								}
							}
                        }
					}
                }
            }
            else
			{
                return false;
			}
        }
        else
		{
            return false;
		}
    }
    else
	{
        return false;
	}

	return false;
}

//------------------------------------------------------------------------------------------------------------------
SDL_Joystick *Joystick;
bool JumpHeuristicEnabled=false;
TTF_Font *font;
SDL_Surface *IMGPanel,*IMGCpu,*IMGPlayer,*IMGPlayerWins,*IMGCpuWins,*IMGDraw;
GameStates GameState = GSTitleScreen;
Mix_Music *Music;
bool TvOutMode = false;
int Difficulty = Hard,Volume=128;

void SetVolume(const int VolumeIn)
{
	Mix_Volume(0,VolumeIn);
	Mix_Volume(1,VolumeIn);
	Mix_VolumeMusic(VolumeIn);
}

void IncVolume()
{
	if (Volume < 128)
	{
		Volume += 8;
		SetVolume(Volume);
	}
}

void DecVolume()
{
	if (Volume > 0)
	{
		Volume-= 8;
		SetVolume(Volume);
	}
}

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

void WriteText(SDL_Surface* Surface,TTF_Font* FontIn,char* Tekst,int NrOfChars,int X,int Y,int YSpacing,SDL_Color ColorIn)
{
	char List[100][255];
	int Lines,Teller,Chars;
	SDL_Rect DstRect;
	SDL_Surface* TmpSurface1;
	Lines = 0;
	Chars = 0;
	for (Teller=0;Teller<NrOfChars;Teller++)
	{
		if(Lines > 100)
			break;
		if((Tekst[Teller] == '\n') || (Chars==255))
		{
			List[Lines][Chars]='\0';
			Lines++;
			Chars = 0;
		}
		else
		{
		 	List[Lines][Chars]=Tekst[Teller];
		 	Chars++;
		}
	}
	List[Lines][Chars] = '\0';
	for (Teller=0;Teller <= Lines;Teller++)
	{
		if(strlen(List[Teller]) > 0)
		{
			TmpSurface1 = TTF_RenderText_Blended(FontIn,List[Teller],ColorIn);
			DstRect.x = X;
			DstRect.y = Y + (Teller) * TTF_FontLineSkip(FontIn) + (Teller*YSpacing);
			DstRect.w = TmpSurface1->w;
			DstRect.h = TmpSurface1->h;
			SDL_BlitSurface(TmpSurface1,NULL,Surface,&DstRect);
			SDL_FreeSurface(TmpSurface1);
		}
	}
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
   Selector = new CSelector(4,3);
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
                    case SDLK_PAGEUP:
						IncVolume();
						break;
					case SDLK_PAGEDOWN:
						DecVolume();
						break;
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
                printf("Game Ended!\n");
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
                            printf("Cpu wins\n");
                            if (GlobalSoundEnabled)
                            {
                                printf("loser sound played \n");
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


void TitleScreen()
{
    SDL_Event Event;
    CMainMenu *Menu = new CMainMenu;
    if (GlobalSoundEnabled)
    {
        if (!Mix_PlayingMusic())
            Mix_PlayMusic(Music,-1);
    }
    while (GameState == GSTitleScreen)
    {
        while(SDL_PollEvent(&Event))
        {
            if (Event.type == SDL_KEYDOWN)
			{
                switch (Event.key.keysym.sym)
                {
                    case SDLK_PAGEUP:
						IncVolume();
						break;
					case SDLK_PAGEDOWN:
						DecVolume();
						break;
                    case SDLK_KP8:
                        Menu->PreviousItem();
                        break;
                    case SDLK_KP2:
                        Menu->NextItem();
                        break;
                    case SDLK_a:
                        if (GlobalSoundEnabled)
                            Mix_PlayChannel(-1,Sounds[SND_SELECT],0);
                        switch(Menu->GetSelection())
                        {
                            case 1:
                                GameState = GSGame;
                                break;
                            case 2:
                                GameState = GSOptions;
                                break;
                            case 3:
                                GameState = GSQuit;
                                break;
                        }
                        break;
                    case SDLK_b:
                        GameState = GSQuit;
					default:
						break;
                }
			}
        }
        Menu->Draw(Screen);
        SDL_Flip(Screen);
    }
    delete Menu;
}

void Options()
{
    SDL_Event Event;
    COptionMenu *Menu = new COptionMenu(Difficulty,JumpHeuristicEnabled);
    while (GameState == GSOptions)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_KEYDOWN)
            {
                switch(Event.key.keysym.sym)
                {
                    case SDLK_PAGEUP:
						IncVolume();
						break;
					case SDLK_PAGEDOWN:
						DecVolume();
						break;
                    case SDLK_KP8:
                        Menu->PreviousItem();
                        break;
                    case SDLK_KP2:
                        Menu->NextItem();
                        break;
                    case SDLK_KP4:
                        Menu->PreviousOption();
                        break;
                    case SDLK_KP6:
                        Menu->NextOption();
                        break;
                    default:
                        GameState = GSTitleScreen;
                }
            }
        }
        Menu->Draw(Screen);
        SDL_Flip(Screen);
    }
    Difficulty = Menu->GetOptionDifficulty();
    JumpHeuristicEnabled = Menu->GetOptionJump();
    delete Menu;
}


void LoadSounds()
{
    if(GlobalSoundEnabled)
    {
        Sounds[SND_START] = Mix_LoadWAV("./Source/sound/start.wav");
		Sounds[SND_SELECT] = Mix_LoadWAV("./Source/sound/select.wav");
		Sounds[SND_TAKEMOVE] = Mix_LoadWAV("./Source/sound/good.wav");
		Sounds[SND_WRONG] = Mix_LoadWAV("./Source/sound/wrong.wav");
		Sounds[SND_WINNER] = Mix_LoadWAV("./Source/sound/winner.wav");
		Sounds[SND_LOSER] = Mix_LoadWAV("./Source/sound/loser.wav");
		Sounds[SND_MENU] = Mix_LoadWAV("./Source/sound/menu.wav");
		Sounds[SND_MOVE] = Mix_LoadWAV("./Source/sound/drop.wav");
        Music = Mix_LoadMUS("./Source/music/checkers.ogg");
    }
}

void UnloadSounds()
{
    if (GlobalSoundEnabled)
    {
        if (!Mix_PlayingMusic())
            Mix_HaltMusic();
        Mix_FreeMusic(Music);
        int Teller;
        for (Teller=0;Teller<NrOfSounds;Teller++)
		if(Sounds[Teller])
			Mix_FreeChunk(Sounds[Teller]);
    }

}

void LoadSettings()
{
 	FILE *SettingsFile;
 	int Value=0;
 	SettingsFile = fopen("./settings.dat","r");
 	if(SettingsFile)
 	{
		fscanf(SettingsFile,"Volume=%d\n",&Volume);
		SetVolume(Volume);
        fscanf(SettingsFile,"Difficulty=%d\n",&Difficulty);
        fscanf(SettingsFile,"JumpHeuristic=%d\n",&Value);
        if (Value == 1)
            JumpHeuristicEnabled = true;
        else
            JumpHeuristicEnabled = false;
		fclose(SettingsFile);
 	}
 	else
 	{
 	    Volume = 128;
		SetVolume(Volume);
        Difficulty = Hard;
        JumpHeuristicEnabled = false;
	}
}

// Save the settings
void SaveSettings()
{
 	FILE *SettingsFile;
 	SettingsFile = fopen("./settings.dat","w");
 	if(SettingsFile)
 	{
		fprintf(SettingsFile,"Volume=%d\n",Volume);
        fprintf(SettingsFile,"Difficulty=%d\n",Difficulty);
        if (JumpHeuristicEnabled)
            fprintf(SettingsFile,"JumpHeuristic=1\n");
        else
            fprintf(SettingsFile,"JumpHeuristic=0\n");
		fclose(SettingsFile);
//		sync();
 	}
}

int main(int argc, char **argv)
{
	if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) == 0)
	{
		printf("SDL Succesfully initialized\n");
        Screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT,16, SDL_SWSURFACE );
		if(Screen)
		{
		    printf("Succesfully Set %dx%dx16\n",WINDOW_WIDTH,WINDOW_HEIGHT);

			SDL_ShowCursor(SDL_DISABLE);
			{
				if (Mix_OpenAudio(22050,AUDIO_S16,MIX_DEFAULT_CHANNELS,1024) < 0)
				{
					GlobalSoundEnabled = false;
					printf("Failed to initialise sound!\n");
					printf("%s\n",Mix_GetError());
				}
				else
				{
					printf("Audio Succesfully initialised!\n");
				}
				if (TTF_Init() == 0)
				{
					printf("Succesfully initialized TTF\n");
					font = TTF_OpenFont("./Source/fonts/font.ttf",18);
					if (font)
					{
						printf("Succesfully Loaded fonts\n");
						TTF_SetFontStyle(font,TTF_STYLE_NORMAL);
						LoadSounds();
						LoadSettings();
						while (GameState != GSQuit)
						{
							switch(GameState)
							{
								case GSGame :
									Game();
									break;
								case GSTitleScreen:
									TitleScreen();
									break;
								case GSOptions:
									Options();
									break;
								default:
									break;
							}
						}
						SaveSettings();
						UnloadSounds();
						Mix_CloseAudio();
						TTF_CloseFont(font);
						font=NULL;
					}
					else
					{
						printf("Failed to Load fonts\n");
					}
					TTF_Quit();
				}
				else
				{
					printf("Failed to initialize TTF\n");
				}
			}
			SDL_FreeSurface(Screen);
            Screen=NULL;
		}
		else
		{
			printf("Failed to Set Videomode %dx%dx16\n",WINDOW_WIDTH, WINDOW_HEIGHT);
		}
		SDL_Quit();
	}
	else
	{
		printf("Couldn't initialise SDL!\n");
	}

	return 0;

}
