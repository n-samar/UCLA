#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "Arena.h"
#include "Player.h"

class Game
{
  public:
      // Constructor/destructor
    Game(int rows, int cols, int nRats);
    ~Game();

      // Mutators
    void play();

  private:
    Arena* m_arena;

      // Helper functions
    std::string takePlayerTurn();
};

#endif