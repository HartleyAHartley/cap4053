#include "minimaxexample.h"

namespace GameAI::GamePlaying::ExampleAI
{
  using Core::ComputerMove,Core::Board;

  // Token: 0x06000005 RID: 5 RVA: 0x000020A3 File Offset: 0x000002A3
  ComputerMove MinimaxExample::Run(int _color, const Core::Board& _board, int _lookAheadDepth)
  {
    return Minimax(_board, _color, _lookAheadDepth);
  }

  // Token: 0x06000006 RID: 6 RVA: 0x000020B0 File Offset: 0x000002B0
  ComputerMove MinimaxExample::Minimax(const Core::Board& A_0, int A_1, int A_2)
  {
    ComputerMove computerMove(-1,-1);
    for (int i = 0; i < 8; i++)
    {
      for (int j = 0; j < 8; j++)
      {
        if (A_0.IsValidMove(A_1, i, j))
        {
          ComputerMove computerMove2 = ComputerMove(i, j);
          Board board = Board(A_0);
          board.MakeMove(A_1, computerMove2.row, computerMove2.column);
          int num = -A_1;
          if (!board.HasAnyValidMove(num))
          {
            num = -num;
          }
          if (board.IsTerminalState() || A_2 == 0)
          {
            computerMove2.rank = Evaluate(board);
          }
          else
          {
            ComputerMove computerMove3 = Minimax(board, num, A_2 - 1);
            computerMove2.rank = computerMove3.rank;
          }
          if (computerMove.row == -1)
          {
            computerMove = computerMove2;
          }
          else if (A_1 * computerMove2.rank > A_1 * computerMove.rank)
          {
            computerMove = computerMove2;
          }
        }
      }
    }
    return computerMove;
  }

  // Token: 0x06000007 RID: 7 RVA: 0x00002180 File Offset: 0x00000380
  int MinimaxExample::Evaluate(const Core::Board& A_0)
  {
    int num = 0;
    for (int i = 0; i < Board::Height; i++)
    {
      for (int j = 0; j < Board::Width; j++)
      {
        if (i == 0 || i == 7)
        {
          if (j == 0 || j == 7)
          {
            num += A_0[i*Board::Width+j] * 100;
          }
          else
          {
            num += A_0[i*Board::Width+j] * 10;
          }
        }
        else if (j == 0 || j == 7)
        {
          num += A_0[i*Board::Width+j] * 10;
        }
        else
        {
          num += A_0[i*Board::Width+j];
        }
      }
    }
    if (A_0.IsTerminalState())
    {
      if (A_0.Score() > 0)
      {
        num += 10000;
      }
      else if (A_0.Score() < 0)
      {
        num -= 10000;
      }
    }
    return num;
  }

}
