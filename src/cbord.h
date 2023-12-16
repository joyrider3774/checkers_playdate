#ifndef CBORD_H
#define CBORD_H

#include "common.h"
class CBord
{
    private:
        int BordArray[8][8];
    public:
        CBord();
        CBord(CBord *Bord);
        void ApplyMove(SMove &move,bool PlaySound);
        void ApplyMoves(vector<SMove> &Moves,bool reverse,bool DelayDraw,bool PlaySound, bool DelayBeforeMove, unsigned int firstDelayDiff);
        void UndoMove(SMove &Move);
        void UndoMoves(vector<SMove> &Moves,bool reverse);
        void Draw(LCDBitmap *Surface);
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
#endif