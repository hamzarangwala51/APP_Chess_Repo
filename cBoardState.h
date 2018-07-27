/*
 * boardState.h
 *
 *  Created on: Jul 25, 2018
 *      Author: mduarte
 */

#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <ctype.h>

#ifndef BOARDSTATE_H_
#define BOARDSTATE_H_

//Player v Player, Player v AI, AI v AI
enum gameMode {PVP = 0, PVA, AVA};

//values that are stored in the board
enum piece {W_KING = -6, W_QUEEN, W_ROOK, W_BISHOP, W_KNIGHT, W_PAWN,
            EMPTY = 0, B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING};


enum list {MOVELIST = 0, VALIDLIST};

enum gameState {PLAYING = 0, DRAW, WHITEWON, BLACKWON};

typedef struct gameMove
{
  int fromI, fromJ; //start and end squares
  int toI, toJ;
  int piece, capture; //piece that moved and piece that was captured
}gameMove;

class cBoardState
{
  private:
    int mTurn;         //turn counter
    int mState;        //gameState value from above
    int mWScore;
    int mBScore;       //total board score, see computeValidMoves()
    int mBoard[8][8];
    int mEnPassant[2]; //coordinates of piece that can be captured en passant
    int mWLostCastle; //what turn did white lose castling ability?
    int mBLostCastle;

    bool mWCheck;      //is white in check?
    bool mBCheck;

    std::vector<gameMove> mMoveList;  //list of moves that have been played
    std::vector<gameMove> mValidList; //list of legal moves that can be played this turn

    void fAddPieceMoves(int i, int j);   //add moves for the piece at the given coordinates
    void fAddPawnMoves(int i, int j);
    void fAddKnightMoves(int i, int j);
    void fAddHorVertMoves(int i, int j); //add horizontal and vertical moves for piece
    void fAddDiagMoves(int i, int j);    //add diagonal moves
    void fAddKingMoves(int i, int j);
    void fAddMove(int i, int j, int i2, int j2, int list); //add move to the given list
    void fPrintMoves();
    std::string fPrintPiece(int piece);
    void fMove(gameMove *m);             //perform the given move on the board
    void fIsInCheck();                   //update values if current player's king is in check
    void fRemoveChecks();				//remove moves that result in check
    bool fCanMove(int i, int j);
    bool fMoveIsValid(gameMove *m);
    bool fCheckMoves(gameMove *m);       //check if the given move is in the validList


    //alpha beta pruning functions, used to find the "best" move the ai can make
    int fAlphaBetaMax(gameMove* m, int maxPlayer, int alpha, int beta, int depthLeft);
    int fAlphaBetaMin(gameMove* m, int maxPlayer, int alpha, int beta, int depthLeft);

  public:
    cBoardState();
    cBoardState(const cBoardState &b2);

    void fComputeValidMoves();
    void fPrintBoard();
    void fProcessMove(gameMove *m);
    void fUndoMove();
    void fCleanup();


    gameMove* fAiCalculateMove();

    int fGetState();
    int fGetTurn();
};

#endif /* BOARDSTATE_H_ */
