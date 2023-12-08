#ifndef COMMON_H 
#define COMMON_H

#include <vector>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

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
        NrOfSounds=9,
        SND_SELECT=0,
		SND_TAKEMOVE=1,
		SND_WRONG=2,
		SND_START=3,
		SND_WINNER=4,
		SND_LOSER=5,
		SND_MENU=6,
		SND_MOVE=7,
		SND_DRAW=8,
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

typedef struct SMove SMove;
struct SMove
{
    int X,Y,NewX,NewY,DeletedPiece,Player,KingX,KingY;
};

typedef struct SValueMove SValueMove;
struct SValueMove
{
    SMove Move;
    int Value;
};

typedef struct SPoint SPoint;
struct SPoint
{
    int X,Y;
};

enum GameStates {GSQuit,GSGame,GSTitleScreen,GSOptions};
enum GameResult {CPUWin,PlayerWin,Draw,Playing};
typedef enum GameStates GameStates;
typedef enum GameResult GameResult;


extern SDL_Surface *Screen, *WhiteSquare,*BlackSquare,*BluePawn,*RedPawn,*RedKing,*BlueKing;
extern Mix_Chunk *Sounds[NrOfSounds];

extern int FunctionCalls;
extern Uint32 Ttime;
extern bool GlobalSoundEnabled;
extern vector<SMove> MadeMoveList;
extern bool JumpHeuristicEnabled;
extern TTF_Font *font;
extern SDL_Surface *IMGPanel,*IMGCpu,*IMGPlayer,*IMGPlayerWins,*IMGCpuWins,*IMGDraw;
extern GameStates GameState;
extern Mix_Music *Music;
extern int Difficulty,Volume;




#endif