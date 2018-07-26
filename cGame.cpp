/*
 * cBoardState.cpp
 *
 *  Created on: Jul 25, 2018
 *      Author: mduarte
 */

#include "cGame.h"
#include <stddef.h>

//make sure only one cGame class can be created
cGame* cGame::mGame = NULL;

cGame::cGame()
{

}

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

//  bs->mValidList.clear();
//  bs->mMoveList.clear();
}

void cGame::fGameLoopPVP()
{
  gameMove m;

  while(bs->fGetState() == PLAYING)
  {
     bs->fPrintBoard();

    if(bs->fGetTurn()%2 == 0)
      std::cout << "White: ";
    else
      std::cout << "Black: ";

    if(fGetCmd(&m))
    {
     bs->fProcessMove(&m); //check if valid coordinates were entered
     //std::cout << "wCheck: = , bcheck: = %d\n", bs->wCheck, bs->bCheck);
    }
    system("clear");
  }

  bs->fPrintBoard();
  switch(bs->fGetState())
  {
    case DRAW:  std::cout << "Draw!" << std::endl; break;

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

int cGame::fGetCmd(gameMove *m)
{
  int i;
  std::string coords;
  std::cin >> coords;


  if(coords.length() != 4)
    return 0;

  for(i = 0; i < 4; i++)
    coords[i] = tolower(coords[i]);

  if(strcmp("quit", coords.c_str()) == 0)
  {
    bs->fCleanup();
    exit(0);
  }
  
//  else if(strcmp("undo", coords) == 0 && bs->turn < 0)
  //  undoMove(&bs, &(bs->moveListTail->move));
  
  if(fCheckCoords(coords))
  {
    m->fromJ = coords[0] - 'a';
    m->fromI = '8' - coords[1];
    m->toJ = coords[2] - 'a';
    m->toI = '8' - coords[3];
    return 1;
  }

  return 0;
}

int cGame::fCheckCoords(std::string coords)
{
  int i;
  for(i = 0; i < 4; i++)
  {
    if(i%2 == 0)
    {
      if( !isalpha(coords[i]) ) //indices 0 and 2 must be letters a-h
        return 0;

      else if( coords[i] > 'h' ) //alpha coords are out of bounds
        return 0;
    }

    else
    {
      if( !isdigit(coords[i]) ) //indices 1 and 3 must be digits 1-8
        return 0;

      else if( coords[i] > '8' )
        return 0;
    }
  }

  return 1;
}
