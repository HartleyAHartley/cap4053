#pragma once
#include "board.h"
#include "computermove.h"
#include "behavior.h"

namespace GameAI::GamePlaying
{

  class StudentAI{
  public:
      Core::ComputerMove Run(int _color, const Core::Board& _board, int _lookAheadDepth);

  private:

      int Minimax(int _color, const Core::Board& _board, int _lookAheadDepth);

      int OtherColor(int _color);

      int Evaluate(const Core::Board& board);

  };

}
