#pragma once
#include "behavior.h"
#include "board.h"
#include "computermove.h"

namespace GameAI::GamePlaying::ExampleAI
{
  class MinimaxExample
  {
    public:
      Core::ComputerMove Run(int _color, const Core::Board& _board, int _lookAheadDepth);
    private:
      Core::ComputerMove Minimax(const Core::Board& A_0, int A_1, int A_2);
      int Evaluate(const Core::Board& A_0);
  };
}
