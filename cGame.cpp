/*
 * cBoardState.cpp
 *
 *  Created on: Jul 25, 2018
 *      Author: mduarte
 */

#include "cGame.h"

//make sure only one cGame class can be created
cGame* cGame::mGame = null;

cGame* cGame::fGetGame()
{
  return mGame ? mGame : (mGame = new cGame());
}

void cGame::fRun(int mode)
{
  bs = new cBoardState();

  sleep(2);
  system("clear");

  if(mode == PVP)
  {
    bs->fComputeValidMoves();
    fGameLoopPVP();
  }
  else if(mode == PVA)
  {
    bs->fComputeValidMoves();
    fGameLoopPVA();
  }
  else
    fGameLoopAVA();

  bs->mValidList.clear();
  bs->mMoveList.clear();
}

void cGame::fGameLoopPVP()
{
  gameMove m;

  while(bs->mState == PLAYING)
  {
     bs->fPrintBoard();

    if(bs->mTurn%2 == 0)
      std::cout << "White: " << std::endl;
    else
      std::cout << "Black: " << std::endl;

    if(getCmd(bs, &m))
    {
     bs->fProcessMove(&m); //check if valid coordinates were entered
     //std::cout << "wCheck: = , bcheck: = %d\n", bs->wCheck, bs->bCheck);
    }
    system("clear");
  }

  bs->fPrintBoard();
  switch(bs->mState)
  {
    case DRAW:  std::cout << "Draw!" << std::cout; break;

    case WHITEWON: std::cout << "White Won!" << std::endl; break;

    case BLACKWON: std::cout << "Black Won!" << std::endl; break;
  }
}

void cGame::fGameLoopPVA()
{

}

void cGame::fGameLoopAVA()
{

}
