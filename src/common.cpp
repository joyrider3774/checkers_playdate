#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "common.h"




SDL_Surface *Screen, *WhiteSquare,*BlackSquare,*BluePawn,*RedPawn,*RedKing,*BlueKing;
Mix_Chunk *Sounds[NrOfSounds];
int FunctionCalls = 0;
Uint32 Ttime=0;
bool GlobalSoundEnabled=true;
vector<SMove> MadeMoveList;
bool JumpHeuristicEnabled=false;
TTF_Font *font;
SDL_Surface *IMGPanel,*IMGCpu,*IMGPlayer,*IMGPlayerWins,*IMGCpuWins,*IMGDraw;
GameStates GameState = GSTitleScreen;
Mix_Music *Music;
int Difficulty = Hard,Volume=128;