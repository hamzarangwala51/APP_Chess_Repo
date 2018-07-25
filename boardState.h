/*
 * boardState.h
 *
 *  Created on: Jul 25, 2018
 *      Author: mduarte
 */

#include <iostream>
#include <string>
#include <vector>

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
  int fromI, toI; //start and end squares
  int fromJ, toJ;
  int piece, capture; //piece that moved and piece that was captured
};

class boardState
{
  private:
	int turn;         //turn counter
	int state;        //gameState value from above
    int wScore;
    int bScore;       //total board score, see computeValidMoves()
	int board[8][8];
	int enPassant[2]; //coordinates of piece that can be captured en passant

	bool wCheck;      //is white in check?
	bool bCheck;
	bool wLostCastle; //can white castle?
	bool bLostCastle;

	std::vector<gameMove> *moveList;  //list of moves that have been played
	std::vector<gameMove> *validList; //list of legal moves that can be played this turn

  public:
	boardState();
	boardState(const boardState &b2);

	void printMoves();
	void printBoard();
	void computeValidMoves();
	void addPieceMoves(int i, int j);   //add moves for the piece at the given coordinates
	void addPawnMoves(int i, int j);
	void addKnightMoves(int i, int j);
	void addHorVertMoves(int i, int j); //add horizontal and vertical moves for piece
	void addDiagMoves(int i, int j);    //add diagonal moves
	void addKingMoves(int i, int j);
	bool checkMoves(gameMove *m);       //check if the given move is in the validList
	bool isMoveValid(gameMove *m);
	void processMove(gameMove *m);
	void move(gameMove *m);             //perform the given move on the board
	void isInCheck();                   //update values if current player's king is in check
	void removeChecks();				//remove moves that result in check
    gameMove* aiCalculateMove();

    //alpha beta pruning functions, used to find the "best" move the ai can make
    int alphaBetaMax(gameMove* m, int maxPlayer, int alpha, int beta, int depthLeft)
    int alphaBetaMin(gameMove* m, int maxPlayer, int alpha, int beta, int depthLeft)
};



#endif /* BOARDSTATE_H_ */
