/*
 * game.h
 *
 *  Created on: Jul 25, 2018
 *      Author: mduarte
 */

#include <iostream>
#include <unistd.h>
#include <string.h>
#include "cBoardState.h"

#ifndef CGAME_H_
#define CGAME_H_

class cGame{
  private:
    cBoardState *bs;

    cGame();
    cGame(const cGame &g2) = delete;
    cGame& operator=(cGame const&){};
    static cGame* mGame;

  public:
    static cGame* fGetGame();
    void fRun(int mode);
    void fGameLoopPVP();
    void fGameLoopPVA();
    void fGameLoopAVA();
    int fGetCmd(gameMove *m);
    int fCheckCoords(std::string coords);
};

#endif /* CGAME_H_ */
