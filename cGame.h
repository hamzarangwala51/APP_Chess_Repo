/*
 * game.h
 *
 *  Created on: Jul 25, 2018
 *      Author: mduarte
 */

#include <iostream>
#include <unistd.h>
#include "cBoardState.h"

#ifndef CGAME_H_
#define CGAME_H_

class cGame{
  private:
    cBoardState *bs;

    cGame();
    cGame(const cGame &g2) = delete;
    static cGame* mGame;

  public:
    cGame* fGetGame();
    void fRun(int mode);
    void fGameLoopPVP();
    void fGameLoopPVA();
    void fGameLoopAVA();
};

#endif /* CGAME_H_ */
