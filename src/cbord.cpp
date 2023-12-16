#include <vector>
#include "cbord.h"
#include "common.h"
#include "pd_helperfuncs.h"
#include "sdl_helpertypes.h"

using namespace std;


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
        if (PlaySound)
            CAudio_PlaySound(Sounds[SND_TAKEMOVE],0);
    }
    else
    {
        if (PlaySound)
            CAudio_PlaySound(Sounds[SND_MOVE],0);
    }
}

void CBord::ApplyMoves(vector<SMove> &Moves,bool reverse,bool DelayDraw,bool PlaySound, bool DelayBeforeMove, unsigned int firstDelayDiff)
{
    vector<SMove>::iterator iter;
    vector<SMove>::reverse_iterator riter;
    if (reverse)
    {
        riter  = Moves.rbegin();
        while (riter != Moves.rend())
        {
			if (DelayBeforeMove)
			{
				if(riter == Moves.rbegin())
					pdDelay(400-firstDelayDiff);
				else
					pdDelay(400);
			}
            ApplyMove(*riter,PlaySound);
            riter++;
            if(DelayDraw && (riter != Moves.rend()))
            {
				Draw(NULL);
				//force update screen
				pd->graphics->display(); 
            }
        }
    }
    else
    {
        iter  = Moves.begin();
        while (iter != Moves.end())
        {
			if (DelayBeforeMove)
			{
				if(iter == Moves.begin())
					pdDelay(400-firstDelayDiff);
				else
					pdDelay(400);
            ApplyMove(*iter,PlaySound);
            iter++;
            if(DelayDraw && (iter != Moves.end()))
            {
				Draw(NULL);
				//force update screen
				pd->graphics->display(); 
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
            UndoMove(*riter);
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

void CBord::Draw(LCDBitmap *Surface)
{
    int X,Y;
    SDL_Rect Rect;
	if(Surface != NULL)
		pd->graphics->pushContext(Surface);
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
                    pd->graphics->drawBitmap(BluePawn, Rect.x, Rect.y, kBitmapUnflipped);
                    break;
                case ComputerPawn:
                    pd->graphics->drawBitmap(RedPawn, Rect.x, Rect.y, kBitmapUnflipped);
                    break;
                case ComputerKing:
                    pd->graphics->drawBitmap(RedKing, Rect.x, Rect.y, kBitmapUnflipped);
                    break;
                case HumanKing:
                    pd->graphics->drawBitmap(BlueKing, Rect.x, Rect.y, kBitmapUnflipped);
                    break;
                case 0:
                    if(((Y*(NrOfRows-1))+X) % 2 == 1)
                        pd->graphics->drawBitmap(BlackSquare, Rect.x, Rect.y, kBitmapUnflipped);
                    else
                        pd->graphics->drawBitmap(WhiteSquare, Rect.x, Rect.y, kBitmapUnflipped);
                    break;
            }
        }
    }
	if(Surface != NULL)
		pd->graphics->popContext();
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
