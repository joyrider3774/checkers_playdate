#include <climits>
#include <cmath>
#include "pd_api.h"
#include "pd_helperfuncs.h"
#include "ccomputerai.h"
#include "common.h"
#include "cbord.h"

// void PrintMoves(vector<vector<SMove> > List)
// {
//     vector<SMove> iter2;
//     size_t x,y;
//     pd->system->logToConsole("%zu\n",List.size());
//     for (y=0;y<List.size();y++)
//     {
//         for (x=0;x<List[y].size();x++)
//         {
//             pd->system->logToConsole("Move %zu (%d,%d) -> (%d,%d) KingX:%d KingY:%d\n",y,List[y][x].X,List[y][x].Y,List[y][x].NewX,List[y][x].NewY,List[y][x].KingX,List[y][x].KingY);
//         }
//     }
// }

ComputerAi::ComputerAi(int SearchDepthIn,bool JumpHeuristicEnabledIn)
{
	MinMaxiter = MinMaxMoves.end();
    SearchDepth=SearchDepthIn;
    JumpHeuristicEnabled = JumpHeuristicEnabledIn;
	madeMove = true;
	lastStatus = AiIdle;
}

ComputerAi::~ComputerAi()
{
	MinMaxiter = MinMaxMoves.end();
	madeMove = true;
	lastStatus = AiIdle;
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
            Bord->ApplyMoves(*Iter,true,false,false,false,0);
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
            Bord->ApplyMoves(*Iter,true,false,false,false,0);
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

AiStatus ComputerAi::Update()
{
    if (lastStatus == AiCalculating)
	{
		if ((MinMaxiter != MinMaxMoves.end()))
		{
			MinMaxBord->ApplyMoves(*MinMaxiter,true,false,false,false,0);

			if (MinMaxPlayer==Computer)
			{
				MinMaxValue = Min(MinMaxBord,MinMaxdepth,INT_MIN,INT_MAX);
				MinMaxBord->UndoMoves(*MinMaxiter,false);
				if(MinMaxValue > MinMaxBestValue)
				{

					MinMaxBestMove = *MinMaxiter;
					MinMaxBestValue = MinMaxValue;

				}
			}
			else
			{
				MinMaxValue = Max(MinMaxBord,MinMaxdepth,INT_MIN,INT_MAX);
				MinMaxBord->UndoMoves(*MinMaxiter,false);
				if(MinMaxValue < MinMaxBestValue)
				{
					MinMaxBestMove = *MinMaxiter;
					MinMaxBestValue = MinMaxValue;
				}
			}

			MinMaxiter++;

			if(MinMaxiter == MinMaxMoves.end())
			{
				vector<vector<SMove> >().swap(MinMaxMoves);
				unsigned int MakeMoveEnd = pd->system->getCurrentTimeMilliseconds();
				MinMaxBord->ApplyMoves(MinMaxBestMove,true,true,true,true, MakeMoveEnd - MakeMoveStart < 400 ? MakeMoveEnd - MakeMoveStart : 0);
				for (size_t Teller=0;Teller<MinMaxBestMove.size();Teller++)
					MadeMoveList.push_back(MinMaxBestMove[Teller]);
				madeMove = (MinMaxBestMove.size() > 0);
				lastStatus = AiHasResult;
				//pd->system->logToConsole("Time Taken to calculate move: %d\n",MakeMoveEnd-MakeMoveStart);

			}
		}
	}
	else
		if(lastStatus == AiHasResult)
			lastStatus = AiIdle;
	
	return lastStatus;	
}



void ComputerAi::MakeMove(CBord *Bord,int Player)
{
    lastStatus = AiCalculating;
	MakeMoveStart = pd->system->getCurrentTimeMilliseconds();
    MinMaxMoves.clear();
    MinMaxBestMove.clear();
	MinMaxMoves = GenerateMovesList(Bord,Player);
	MinMaxdepth = SearchDepth;
	MinMaxPlayer = Player;
	MinMaxBord = Bord;
	madeMove = true;
	//if(Player == Human)
    //    PrintMoves(Moves);
    MinMaxiter = MinMaxMoves.begin();
    if(Player == Computer)
        MinMaxBestValue = INT_MIN;
    else
        MinMaxBestValue = INT_MAX;
}
