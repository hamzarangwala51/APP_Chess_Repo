
#include <string.h>
#include "cGame.h"

int main(int argc, char **argv)
{

  if(argc == 1)
  {
    std::cout << "Entering Player vs Player mode" << std::endl;
      cGame::fGetGame()->fRun(PVP);
  }
  else if(argc == 2)
  {
    if(strcmp(argv[1], "pva") == 0)
    {
       std::cout << "Entering Player vs AI mode" << std::endl;
       cGame::fGetGame()->fRun(PVA);

    }
    else if(strcmp(argv[1], "ava") == 0)
    {
      std::cout << "Entering AI vs AI mode\n" << std::endl;
      cGame::fGetGame()->fRun(AVA);
    }
    else
      std::cout << "usage: " << argv[0] << "<optional args>\n <optional args> = pva , ava.\n pva = Player vs AI. ava = AI vs AI\n" << std::endl;
  }
  else
    std::cout << "usage: " << argv[0] << "<optional args>\n <optional args> = pva , ava.\n pva = Player vs AI. ava = AI vs AI\n" << std::endl;

  cGame::fEndGame();
  return 0;
}
