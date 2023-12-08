#ifndef CCOMPUTERAI_H
#define CCOMPUTERAI_H

#include <vector>
#include "cbord.h"

using namespace std;

enum AiStatus {AiIdle, AiCalculating, AiHasResult};

class ComputerAi
{   
	protected:
		int SearchDepth;
        bool JumpHeuristicEnabled;
        int EndGame(CBord *Bord,int Player);
        float Distance(int X1,int Y1,int X2,int Y2);
    private:
		AiStatus lastStatus;
		unsigned int MakeMoveStart;
		vector<vector<SMove> >MinMaxMoves;
    	int MinMaxBestValue,MinMaxValue,MinMaxdepth,MinMaxPlayer;
    	vector<SMove> MinMaxBestMove;
    	vector<vector<SMove> >::iterator MinMaxiter;
		CBord *MinMaxBord;
        int Max(CBord *Bord,int depth,int alpha,int beta);
        int Min(CBord *Bord,int depth,int alpha,int beta);
        vector<SMove> MinMax(CBord *Bord,int depth,int Player);
        virtual int EvaluateBord(CBord *Bord,int Player);
        vector<vector<SMove> > GenerateMovesList(CBord *Bord,int Player);
    public:
		bool madeMove;
        ComputerAi(int SearchDepthIn,bool JumpHeuristicEnabledIn);
        void MakeMove(CBord *Bord,int Player);
		AiStatus Update();
        virtual ~ComputerAi();
};

#endif