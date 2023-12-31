#include <pd_api.h>
#include "common.h"
#include "cinput.h"


LCDBitmap *WhiteSquare,*BlackSquare,*BluePawn,*RedPawn,*RedKing,*BlueKing;
int Sounds[NrOfSounds];
vector<SMove> MadeMoveList;
bool JumpHeuristicEnabled=false, SoundEnabled=true, MusicEnabled=true;
LCDFont *font;
LCDBitmap *IMGPanel,*IMGCpu,*IMGPlayer,*IMGPlayerWins,*IMGCpuWins,*IMGDraw,*IMGConfirmQuit;
GameStates GameState = GSTitleScreenInit;
int Music;
int Difficulty = Hard;
CInput *Input;
unsigned int FrameTime = 0, Frames = 0; 
float CurrentMs = 0;