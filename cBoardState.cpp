
/*
 * cBoardState.cpp
 *
 *  Created on: Jul 26, 2018
 *      Author: mduarte
 */

#include "cBoardState.h"

cBoardState::cBoardState()
{
  int board[8][8] = {{B_ROOK, B_KNIGHT, B_BISHOP, B_QUEEN, B_KING, B_BISHOP, B_KNIGHT, B_ROOK},
                     {B_PAWN, B_PAWN,   B_PAWN,   B_PAWN,  B_PAWN, B_PAWN,   B_PAWN,   B_PAWN},
                     {EMPTY,  EMPTY,    EMPTY,    EMPTY,   EMPTY,  EMPTY,    EMPTY,    EMPTY},
                     {EMPTY,  EMPTY,    EMPTY,    EMPTY,   EMPTY,  EMPTY,    EMPTY,    EMPTY},
                     {EMPTY,  EMPTY,    EMPTY,    EMPTY,   EMPTY,  EMPTY,    EMPTY,    EMPTY},
                     {EMPTY,  EMPTY,    EMPTY,    EMPTY,   EMPTY,  EMPTY,    EMPTY,    EMPTY},
                     {W_PAWN, W_PAWN,   W_PAWN,   W_PAWN,  W_PAWN, W_PAWN,   W_PAWN,   W_PAWN},
                     {W_ROOK, W_KNIGHT, W_BISHOP, W_QUEEN, W_KING, W_BISHOP, W_KNIGHT, W_ROOK}};

  memcpy(mBoard, board, sizeof(board));

  mTurn = 0;
  mState = PLAYING;
  mWScore = 0;
  mBScore = 0;
  mEnPassant[0] = -1;
  mEnPassant[1] = -1;
  mBLostCastle = -1;
  mWLostCastle = -1;

  mBCheck = false;
  mWCheck = false;

}

cBoardState::cBoardState(const cBoardState &b2)
{
  memcpy(mBoard, b2.mBoard, sizeof(b2.mBoard));
  memcpy(mEnPassant, b2.mEnPassant, sizeof(b2.mEnPassant));

  mTurn = b2.mTurn;
  mState = b2.mState;
  mWScore = b2.mWScore;
  mBScore = b2.mBScore;
  mBCheck = b2.mBCheck;
  mWCheck = b2.mWCheck;
  mBLostCastle = b2.mBLostCastle;
  mWLostCastle = b2.mWLostCastle;
}

void cBoardState::fComputeValidMoves()
{
  int i, j, piece, val;

  mWScore = 0;
  mBScore = 0;

  mValidList.clear();

  for(i = 0; i < 8; i++)
  {
    for(j = 0; j < 8; j++)
    {
      piece = abs(mBoard[i][j]); //absolute value
      switch(piece) //both players will start at 1000 points
      {
         case B_KING:    val = 500;
                         break;

         case B_QUEEN:   val = 300;
                         break;

         case B_ROOK:    val = 50;
                         break;

         case B_BISHOP:  val = 20;
                         break;

         case B_KNIGHT:  val = 10;
                         break;

         case B_PAWN:    val = 5;
                         break;

         case EMPTY: //fall through

         default:        val = 0;
      }

      if(mBoard[i][j] > 0) //black piece
         mBScore += val;
      else //white piece
         mWScore += val;

      if((mTurn%2 == 0 && mBoard[i][j] < EMPTY) || (mTurn%2 == 1 && mBoard[i][j] > EMPTY))
        fAddPieceMoves(i, j);
    }
  }
}

void cBoardState::fAddPieceMoves(int i, int j)
{
  int piece = abs(mBoard[i][j]);

  switch(piece)
  {
    case B_KING:    fAddKingMoves(i, j);
                    break;

    case B_QUEEN:   fAddDiagMoves(i, j);
                    fAddHorVertMoves(i, j);
                    break;

    case B_ROOK:    fAddHorVertMoves(i, j);
                    break;

    case B_BISHOP:  fAddDiagMoves(i, j);
                    break;

    case B_KNIGHT:  fAddKnightMoves(i, j);
                    break;

    case B_PAWN:    fAddPawnMoves(i, j);
                    break;
  }
}

void cBoardState::fAddPawnMoves(int i, int j)
{
  if(mTurn%2 == 0) //white
  {
    //move forward 1
    if(mBoard[i - 1][j] == EMPTY && fCanMove(i - 1, j))
      fAddMove(i, j, i - 1, j, VALIDLIST);

    //move forward 2 for pawns that haven't moved
    if(i == 6 && mBoard[i - 1][j] == EMPTY && mBoard[i - 2][j] == EMPTY)
      fAddMove(i, j, i - 2, j, VALIDLIST);

    //attack forward-right
    if(mBoard[i - 1][j + 1] > EMPTY && fCanMove(i - 1, j + 1))
      fAddMove(i, j, i - 1, j + 1, VALIDLIST);

    //attack forward-left
    if(mBoard[i - 1][j - 1] > EMPTY && fCanMove(i - 1, j - 1))
      fAddMove(i, j, i - 1, j - 1, VALIDLIST);

    //en passant right
    if(mBoard[i - 1][j + 1] == EMPTY && fCanMove(i - 1, j + 1) && mEnPassant[0] == i && mEnPassant[1] == j + 1)
      fAddMove(i, j, i - 1, j + 1, VALIDLIST);

    //en passant left
    if(mBoard[i - 1][j - 1] == EMPTY && fCanMove(i - 1, j - 1) && mEnPassant[0] == i && mEnPassant[1] == j - 1)
      fAddMove(i, j, i - 1, j - 1, VALIDLIST);
  }

  if(mTurn%2 == 1) //black
  {
    //move forward 1
    if(mBoard[i + 1][j] == EMPTY && fCanMove(i + 1, j))
      fAddMove(i, j, i + 1, j, VALIDLIST);

    //move forward 2 for pawns that haven't moved
    if(i == 1 && mBoard[i + 1][j] == EMPTY && mBoard[i + 2][j] == EMPTY)
      fAddMove(i, j, i + 2, j, VALIDLIST);

    //attack forward-right
    if(mBoard[i + 1][j + 1] < EMPTY && fCanMove(i + 1, j + 1))
      fAddMove(i, j, i + 1, j + 1, VALIDLIST);

    //attack forward-left
    if(mBoard[i + 1][j - 1] < EMPTY && fCanMove(i + 1, j - 1))
      fAddMove(i, j, i + 1, j - 1, VALIDLIST);

    //en passant right
    if(mBoard[i + 1][j + 1] == EMPTY && fCanMove(i + 1, j + 1) && mEnPassant[0] == i && mEnPassant[1] == j + 1)
      fAddMove(i, j, i + 1, j + 1, VALIDLIST);

    //en passant left
    if(mBoard[i + 1][j - 1] == EMPTY && fCanMove(i + 1, j - 1) && mEnPassant[0] == i && mEnPassant[1] == j - 1)
      fAddMove(i, j, i + 1, j - 1, VALIDLIST);
  }
}

void cBoardState::fAddKnightMoves(int i, int j)
{
  //move down2-right1
  if(fCanMove(i + 2, j + 1))
    fAddMove(i, j, i + 2, j + 1, VALIDLIST);

  //move down2-left1
  if(fCanMove(i + 2, j - 1))
    fAddMove(i, j, i + 2, j - 1, VALIDLIST);

  //move left2-down1
  if(fCanMove(i + 1, j - 2))
    fAddMove(i, j, i + 1, j - 2, VALIDLIST);

  //move left2-up1
  if(fCanMove(i - 1, j - 2))
    fAddMove(i, j, i - 1, j - 2, VALIDLIST);

  //move up2-left1
  if(fCanMove(i - 2, j - 1))
    fAddMove(i, j, i - 2, j - 1, VALIDLIST);

  //move up2-right1
  if(fCanMove(i - 2, j + 1))
    fAddMove(i, j, i - 2, j + 1, VALIDLIST);

  //move right2-up1
  if(fCanMove(i - 1, j + 2))
    fAddMove(i, j, i - 1, j + 2, VALIDLIST);

  //move right2-down1
  if(fCanMove(i + 1, j + 2))
    fAddMove(i, j, i + 1, j + 2, VALIDLIST);
}

void cBoardState::fAddHorVertMoves(int i, int j)
{
  int i2 , j2;

  //move up
  i2 = i - 1;
  j2 = j;
  while(fCanMove(i2, j2))
  {
    fAddMove(i, j, i2, j2, VALIDLIST);
    if(mBoard[i2][j2] != EMPTY) //blocked by enemy;
      break;
    i2--;
  }

  //move right
  i2 = i;
  j2 = j + 1;
  while(fCanMove(i2, j2))
  {
    fAddMove(i, j, i2, j2, VALIDLIST);
    if(mBoard[i2][j2] != EMPTY) //blocked by enemy;
      break;
    j2++;
  }

  //move down
  i2 = i + 1;
  j2 = j ;
  while(fCanMove(i2, j2))
  {
    fAddMove(i, j, i2, j2, VALIDLIST);
    if(mBoard[i2][j2] != EMPTY) //blocked by enemy;
      break;
    i2++;
  }

  //move left
  i2 = i;
  j2 = j - 1;
  while(fCanMove(i2, j2))
  {
    fAddMove(i, j, i2, j2, VALIDLIST);
    if(mBoard[i2][j2] != EMPTY) //blocked by enemy;
      break;
    j2--;
  }
}

void cBoardState::fAddDiagMoves(int i, int j)
{
  int i2 , j2;

  //move up-right
  i2 = i - 1;
  j2 = j + 1;
  while(fCanMove(i2, j2))
  {
    fAddMove(i, j, i2, j2, VALIDLIST);
    if(mBoard[i2][j2] != EMPTY) //blocked by enemy;
      break;

    i2--;
    j2++;
  }

  //move down-right
  i2 = i + 1;
  j2 = j + 1;
  while(fCanMove(i2, j2))
  {
    fAddMove(i, j, i2, j2, VALIDLIST);
    if(mBoard[i2][j2] != EMPTY) //blocked by enemy;
      break;
    i2++;
    j2++;
  }

  //move down-left
  i2 = i + 1;
  j2 = j - 1;
  while(fCanMove(i2, j2))
  {
    fAddMove(i, j, i2, j2, VALIDLIST);
    if(mBoard[i2][j2] != EMPTY) //blocked by enemy;
      break;
    i2++;
    j2--;
  }

  //move up-left
  i2 = i - 1;
  j2 = j - 1;
  while(fCanMove(i2, j2))
  {
    fAddMove(i, j, i2, j2, VALIDLIST);
    if(mBoard[i2][j2] != EMPTY) //blocked by enemy;
      break;
    i2--;
    j2--;
  }
}

void cBoardState::fAddKingMoves(int i, int j)
{
  //move up
  if(fCanMove(i - 1 , j))
    fAddMove(i, j, i - 1, j, VALIDLIST);

  //move up-right
  if(fCanMove(i - 1, j + 1))
    fAddMove(i, j, i - 1, j + 1, VALIDLIST);

  //move right
  if(fCanMove(i, j + 1))
    fAddMove(i, j, i, j + 1, VALIDLIST);

  //move down-right
  if(fCanMove(i + 1, j + 1))
    fAddMove(i, j, i + 1, j + 1, VALIDLIST);

  //move down
  if(fCanMove(i + 1, j))
    fAddMove(i, j, i + 1, j, VALIDLIST);

  //move down-left
  if(fCanMove(i + 1, j - 1))
    fAddMove(i, j, i + 1, j - 1, VALIDLIST);

  //move left
  if(fCanMove(i, j - 1))
    fAddMove(i, j, i, j - 1, VALIDLIST);

  //move up-left
  if(fCanMove(i - 1, j - 1))
    fAddMove(i, j, i - 1, j - 1, VALIDLIST);

  //castling
  if((mTurn%2 == 0 && mWLostCastle == -1) || (mTurn%2 == 1 && mBLostCastle == -1))
  {
    //castle left
    if(mBoard[i][j-1] == EMPTY && mBoard[i][j-2] == EMPTY && mBoard[i][j-3] == EMPTY && abs(mBoard[i][j-4]) == B_ROOK)
      fAddMove(i, j, i, j - 2, VALIDLIST);

    //castle right
    if(mBoard[i][j+1] == EMPTY && mBoard[i][j+2] == EMPTY && abs(mBoard[i][j+3]) == B_ROOK)
      fAddMove(i, j, i, j + 2, VALIDLIST);
  }
}

void cBoardState::fAddMove(int i, int j, int i2, int j2, int list)
{
  //                                             piece         capture
  gameMove newMove;
  newMove.fromI = i;
  newMove.fromJ = j;
  newMove.toI = i2;
  newMove.toJ = j2;
  newMove.piece = mBoard[i][j];
  newMove.capture = mBoard[i2][j2];

  if(list == MOVELIST)
    mMoveList.push_back(newMove);
  else
    mValidList.push_back(newMove);
}

void cBoardState::fPrintMoves()
{

}

std::string cBoardState::fPrintPiece(int piece)
{
  switch(piece)
  {
    case W_KING:   return "\u2654";
    case W_QUEEN:  return "\u2655";
    case W_ROOK:   return "\u2656";
    case W_BISHOP: return "\u2657";
    case W_KNIGHT: return "\u2658";
    case W_PAWN:   return "\u2659";
    case B_KING:   return "\u265a";
    case B_QUEEN:  return "\u265b";
    case B_ROOK:   return "\u265c";
    case B_BISHOP: return "\u265d";
    case B_KNIGHT: return "\u265e";
    case B_PAWN:   return "\u265f";
    case EMPTY:    return " ";
    default:       return " ";
  }
}

void cBoardState::fPrintBoard()
{
  int i, j;

  std::cout << "     a    b    c    d    e    f    g    h" << std::endl;
  for(i = 0; i < 8; i ++)
  {
    std::cout << "   -----------------------------------------" << std::endl;
    std::cout << " " << (8-i) << " ";

    for(j = 0; j < 8; j++)
      std::cout << "| " << fPrintPiece(mBoard[i][j]) << "  ";

    std::cout << "| " << (8-i) << std::endl;
  }

  std::cout << "   -----------------------------------------" << std::endl;
  std::cout << "     a    b    c    d    e    f    g    h" << std::endl;
}

void cBoardState::fProcessMove(gameMove *m)
{
  if(fMoveIsValid(m))
  {
    fMove(m); //move and update turn
    fIsInCheck();
    fComputeValidMoves();
    fRemoveChecks();
    fPrintBoard();
  }
  else
    std::cout << "Invalid Move!" << std::endl;
}

void cBoardState::fMove(gameMove *m)
{
  int piece = mBoard[m->fromI][m->fromJ];
  int capture = mBoard[m->toI][m->toJ];

  fAddMove(m->fromI, m->fromJ, m->toI, m->toJ, MOVELIST); //keep track of all moves

  mBoard[m->fromI][m->fromJ] = EMPTY; //move piece
  mBoard[m->toI][m->toJ] = piece;

  //check for pawn promotion
  if(piece == W_PAWN && m->toI == 0)
    mBoard[m->toI][m->toJ] = W_QUEEN;

  if(piece == B_PAWN && m->toI == 7)
    mBoard[m->toI][m->toJ] = B_QUEEN;

  //if a pawn moved forward two spaces it is vulnerable to en passant capture
  if(abs(piece) == B_PAWN && abs(m->toI - m->fromI) == 2)
  {
    mEnPassant[0] = m->toI;
    mEnPassant[1] = m->toJ;
  }
  else //en passant capture can only be performed the turn immediately after the pawn moves forward twice
  {
    mEnPassant[0] = -1;
    mEnPassant[1] = -1;
  }

  //check for en passant captures
  if(abs(piece) == B_PAWN && m->fromJ != m->toJ && capture == EMPTY) //pawn moved diagonally to an empty square -> en passant capture
  {
    int enPCapture = mBoard[m->fromI][m->toJ];
    mBoard[m->fromI][m->toJ] = EMPTY;
  }

  //check for castling and move rook
  int len = m->toJ - m->fromJ; //how far did the piece move horizontally
  if(abs(piece) == B_KING && abs(len) == 2)
  {
    if(len > 0) // castled to the right
    {
      int castle = mBoard[m->fromI][m->toJ+1];
      //fAddMove(m->fromI, m->toJ + 1, m->fromI, m->toJ - 1, MOVELIST);
      mBoard[m->fromI][m->toJ+1] = EMPTY;
      mBoard[m->fromI][m->toJ-1] = castle;
    }
    else  //castled to the left
    {
      int castle = mBoard[m->fromI][m->toJ-2];
      //fAddMove(m->fromI, m->toJ-2 , m->fromI, m->toJ + 1, MOVELIST);
      mBoard[m->fromI][m->toJ-2] = EMPTY;
      mBoard[m->fromI][m->toJ+1] = castle;
    }

    if(piece < EMPTY) //white
      mWLostCastle = mTurn;

    else if(piece > EMPTY) //black
      mBLostCastle = mTurn;
  }

  else if(abs(piece) == B_KING) // if king moved but not to castle, don't allow castling
  {
    if(piece < EMPTY && mWLostCastle == -1) // white
      mWLostCastle = mTurn;

    else if(piece > EMPTY && mBLostCastle == -1) //black
      mBLostCastle = mTurn;
  }

  else if(abs(piece) == B_ROOK) // if rook moved
  {
    if(piece < EMPTY && mWLostCastle == -1) //white and has not lost ability to castle
    {
      if(mBoard[7][0] != W_ROOK && mBoard[7][7] != W_ROOK) //if both rooks have moved, no castling
        mWLostCastle = mTurn;
    }

    else if(piece > EMPTY && mBLostCastle == -1) //black
    {
      if(mBoard[0][0] != B_ROOK && mBoard[0][7] != B_ROOK)
        mWLostCastle = mTurn;
    }
  }

mTurn++;
}

void cBoardState::fIsInCheck()
{
  int i;
  gameMove m;
  cBoardState nextBoard = *this;

  nextBoard.mTurn++;

  //check the possible moves the opponent can make
  nextBoard.fComputeValidMoves();

  int loop = nextBoard.mValidList.size();
  for(i = 0; i < loop; i++)
  {
    //if they can capture my king then i am in check
    if(abs(nextBoard.mValidList[i].capture) == B_KING)
    {
      if(nextBoard.mTurn%2 == 0)
        mBCheck = true;

      else if(nextBoard.mTurn%2 == 1)
        mWCheck = true;

      break;
    }
    else
    {
      if(nextBoard.mTurn%2 == 0)
        mBCheck = false;

      else if(nextBoard.mTurn%2 == 1)
        mWCheck = false;
    }
  }

  nextBoard.mValidList.clear();
  nextBoard.mMoveList.clear();
}

void cBoardState::fRemoveChecks()
{
  int i, j;
  gameMove m;

  cBoardState nextBoard = *this;

  std::vector<gameMove>::iterator it;
  for(it = mValidList.begin(); it != mValidList.end(); it++) //loop through all valid moves
  {
    m.fromI = it->fromI;
    m.fromJ = it->fromJ;
    m.toI = it->toI;
    m.toJ = it->toJ;
    m.piece = it->piece;
    m.capture = it->capture;

    nextBoard.fMove(&m); //do the move on the board copy
    nextBoard.fComputeValidMoves();

    std::vector<gameMove>::iterator it2;
    for(it2 = nextBoard.mValidList.begin(); it2 != nextBoard.mValidList.end(); it2++) //loop through all the valid moves in the simulated board
    {
      if(abs(it2->capture) == B_KING) //if the king can be captured, the current move results in a check
      {
        mValidList.erase(it); // so we remove it from the list of valid moves
        it--;
        break; //stop looking in this scenario
      }
    }

    nextBoard.fUndoMove();
  }

  if(mValidList.size() == 0) //no legal moves
  {
    if(mTurn%2 == 0 && mWCheck) //white turn and in check
      mState = BLACKWON;
    else if (mTurn%2 == 1 && mBCheck) //black turn and in check
      mState = WHITEWON;

      //white turn and not in check or black turn and not in check
    else if((mTurn%2 == 0 && !mWCheck) || (mTurn%2 == 1 && !mBCheck))
      mState = DRAW;
  }
  else if(mValidList.size() > 0)
  {
    if(mTurn%2 == 0)
      mWCheck = false;
    else
      mBCheck = false;
  }

  nextBoard.mValidList.clear();
  nextBoard.mMoveList.clear();
}

void cBoardState::fUndoMove()
{
  mTurn--;

  //check if move was an en passant capture, if so J values will not be the same and the capture will be empty
  if(abs(mMoveList.back().piece) == B_PAWN && mMoveList.back().fromJ != mMoveList.back().toJ && mMoveList.back().capture == EMPTY)
  {
    mBoard[mMoveList.back().fromI][mMoveList.back().toJ] = -(mMoveList.back().piece); //captured piece will always be the opposite colored pawn
    mEnPassant[0] = mMoveList.back().fromI;
    mEnPassant[1] = mMoveList.back().toJ;
  }
  else
  {
    mEnPassant[0] = -1;
    mEnPassant[1] = -1;
  }

  //check for castling and fix rook
  if(mMoveList.back().piece == B_KING)
  {
    if (mMoveList.back().toJ - mMoveList.back().fromJ == -2) //castled left
    {
      mBoard[0][0] = B_ROOK;
      mBoard[0][3] = EMPTY;
    }
    else if (mMoveList.back().toJ - mMoveList.back().fromJ == 2) //castled right
    {
      mBoard[0][7] = B_ROOK;
      mBoard[0][5] = EMPTY;
    }
  }
  else if(mMoveList.back().piece == W_KING)
  {
    if (mMoveList.back().toJ - mMoveList.back().fromJ == -2) //castled left
    {
      mBoard[7][0] = W_ROOK;
      mBoard[7][3] = EMPTY;
    }
    else if (mMoveList.back().toJ - mMoveList.back().fromJ == 2) //castled right
    {
      mBoard[7][7] = W_ROOK;
      mBoard[7][5] = EMPTY;
    }
  }

  //reallow castling
  if(mBLostCastle == mTurn)
    mBLostCastle = -1;

  if(mWLostCastle == mTurn)
    mWLostCastle = -1;

  //undo move
  mBoard[mMoveList.back().fromI][mMoveList.back().fromJ] = mMoveList.back().piece;
  mBoard[mMoveList.back().toI][mMoveList.back().toJ] = mMoveList.back().capture;


  mMoveList.pop_back(); //remove move from movelist
}

void cBoardState::fCleanup()
{
  mMoveList.clear();
  mValidList.clear();
}

bool cBoardState::fCanMove(int i, int j)
{
  if( i > 7 || j > 7 || i < 0 || j < 0 ) //out of bounds
  {
    return false;
  }

  int piece = mBoard[i][j];
  //trying to take your own piece or they're in the way
  //      black                              white
  if((piece > EMPTY && mTurn%2 == 1) || (piece < EMPTY && mTurn%2 == 0))
  {
    return 0;
  }

  else if (piece == EMPTY) //can move to empty space
  {
    return true;
  }

  else //enemy piece
  {
    return true;
  }
}

bool cBoardState::fMoveIsValid(gameMove *m)
{
  m->piece = mBoard[m->fromI][m->fromJ];
  m->capture = mBoard[m->toI][m->toJ];

  if(m->piece == EMPTY) //player tries to move an empty square
    return false;

  if(mTurn%2 == 0 && m->piece > EMPTY) //white player tries to move black piece
    return false;

  if(mTurn%2 == 1 && m->piece < EMPTY) //black player tries to move white piece
    return false;

  return fCheckMoves(m);
}

bool cBoardState::fCheckMoves(gameMove *m)
{
  int i, loop = mValidList.size();
  for(i = 0; i < loop; i++)
  {
    if(m->fromJ == mValidList[i].fromJ && m->fromI == mValidList[i].fromI \
       && m->toJ == mValidList[i].toJ && m->toI == mValidList[i].toI )
     //  && m->piece == curr->move.piece && m->capture == curr->move.capture)
      {
       return true;
      }
  }
  return false;
}

gameMove* cBoardState::fAiCalculateMove()
{
  int i, score;
  int bestScore, player;
  std::vector<gameMove>::iterator it, bestMove;

  gameMove *m = new gameMove;

  cBoardState simuBoard = *this;

  if(mValidList.size() == 0)
  {
    if(mTurn%2 == 0 && mWCheck) //white turn and in check
    {
      m->fromI = m->fromJ = m->toI = m->toJ = m->piece = m->capture = -1;//returning arbitrary moves to check outside this function for end game
      mState = BLACKWON;
      return m;
    }
    else if (mTurn%2 == 1 && mBCheck) //black turn and in check 
    {
      m->fromI = m->fromJ = m->toI = m->toJ = m->piece = m->capture = -2;
      mState = WHITEWON;
      return m;
    }
      //white turn and not in check or black turn and not in check
    else if((mTurn%2 == 0 && !mWCheck) || (mTurn%2 == 1 && !mBCheck))
    {
      m->fromI = m->fromJ = m->toI = m->toJ = m->piece = m->capture = -3;
      mState = DRAW;
      return m;
    }
  }

  if(mTurn%2 == 0)
    player = W_PAWN;
  else
    player = B_PAWN;

  bestScore = -9999; //Ai wants to maximize this score. Score is (ai's score - opponent's score)

  //simulate all the moves in the valid list to find the best one
  for(it = mValidList.begin(); it != mValidList.end(); it++)
  {
   // std::cout << "list size = " << mMoveList.size() << std::endl;

    m->fromI = it->fromI;
    m->fromJ = it->fromJ;
    m->toI = it->toI;
    m->toJ = it->toJ;
    m->piece = it->piece;
    m->capture = it->capture;

//  std::cout << "move before undo {" << m->fromI << ", " << m->fromJ << ", " << m->toI << ", " << m->toJ << ", " << m->piece << ", " << m->capture << "}" << std::endl;

    score = simuBoard.fAlphaBeta(m, player, true, -9999, 9999, 6);

    simuBoard.fUndoMove();
    ///*simuBoard->*/fIsInCheck();
    ///*simuBoard->*/fComputeValidMoves();
    ///*simuBoard->*/fRemoveChecks();
//  std::cout << "move after undo {" << it->fromI << ", " << it->fromJ << ", " << it->toI << ", " << it->toJ << ", " << it->piece << ", " << it->capture << "}" << std::endl;

    if(score >= bestScore) // find a better move
    {
      bestScore = score;
      bestMove = it;
    }
  }

  m->fromI = bestMove->fromI;
  m->fromJ = bestMove->fromJ;
  m->toI = bestMove->toI;
  m->toJ = bestMove->toJ;
  m->piece = bestMove->piece;
  m->capture = bestMove->capture;

  simuBoard.fCleanup();
  //delete[] simuBoard;

  std::cout << "returning {" << m->fromI << ", " << m->fromJ << ", " << m->toI << ", " << m->toJ << ", " << m->piece << ", " << m->capture << "}" << std::endl;
  return m; //ai will play this move
}

int cBoardState::fAlphaBeta(gameMove* m, int maxPlayer, bool isMaxPlayer, int alpha, int beta, int depthLeft)
{
  int i, score;
  int bestVal = -9999; //the maximizing player tries to increase this
  std::vector<gameMove>::iterator it;
  gameMove *mv = new gameMove;

  if(depthLeft == 0)
  {
    delete mv;
    mv = 0;

    return (maxPlayer * (mBScore - mWScore));
  }

  fMove(m);
  fIsInCheck();
  fComputeValidMoves();
  fRemoveChecks();


  cBoardState simuBoard = *this;
  int numMoves = mMoveList.size();

  if(isMaxPlayer)
  {
    for(it = mValidList.begin(); it != mValidList.end(); it++)
    {
      mv->fromI = it->fromI;
      mv->fromJ = it->fromJ;
      mv->toI = it->toI;
      mv->toJ = it->toJ;
      mv->piece = it->piece;
      mv->capture = it->capture;

      score = simuBoard.fAlphaBeta(mv, maxPlayer, false, alpha, beta, depthLeft - 1);


      if(depthLeft > 1)
        simuBoard.fUndoMove();

      if(score > bestVal) //is this score better than the current best?
        bestVal = score;

      if(bestVal > alpha) //is the current best score better than the alpha?
        alpha = bestVal;

      if(beta <= alpha)  //is the minimizing player's score better? if so stop looking
      {
        delete mv;
        mv = 0;
        return alpha;
      }
    }

    delete mv;
    mv = 0;
    return alpha;
  }

  else
  {
    for(it = mValidList.begin(); it != mValidList.end(); it++)
    {
      mv->fromI = it->fromI;
      mv->fromJ = it->fromJ;
      mv->toI = it->toI;
      mv->toJ = it->toJ;
      mv->piece = it->piece;
      mv->capture = it->capture;

      score = simuBoard.fAlphaBeta(mv, maxPlayer, true, alpha, beta, depthLeft - 1);

      if(depthLeft > 1)
        simuBoard.fUndoMove();

      if(score < bestVal) //is this score better than the current best?
        bestVal = score;

      if(bestVal < beta) //is the current best score better than the alpha?
        beta = bestVal;

      if(beta <= alpha)  //is the minimizing player's score better? if so stop looking
      {
        delete mv;
        mv = 0;
        return beta;
      }
    }

    delete mv;
    mv = 0;
    return beta;
  }
}

int cBoardState::fGetState(){return mState;}

int cBoardState::fGetTurn(){return mTurn;}


