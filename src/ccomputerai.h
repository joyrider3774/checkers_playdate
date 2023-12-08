#ifndef CCOMPUTERAI_H
#define CCOMPUTERAI_H

#include <vector>
#include "cbord.h"

using namespace std;

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

#endif