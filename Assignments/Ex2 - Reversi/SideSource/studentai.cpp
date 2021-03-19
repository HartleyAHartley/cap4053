#include "studentai.h"
#include <numeric>
#include <vector>

namespace GameAI::GamePlaying
{
  using Core::ComputerMove, Core::Board;

  ComputerMove StudentAI::Run(int _color, const Board& _board, int _lookAheadDepth)
  {
    ComputerMove move = ComputerMove(-1,-1);
    move.rank = std::numeric_limits<int>::min();
    for (int row = 0; row < Board::Height; row++)
    {
      for (int col = 0; col < Board::Width; col++)
      {
        if (_board.IsValidMove(_color, row, col))
        {
          Board b = Board();
          b.Copy(_board);
          b.MakeMove(_color, row, col);
          int tmpvalue = Minimax(OtherColor(_color), b, _lookAheadDepth);
          if(tmpvalue > move.rank)
          {
            move.row = row;
            move.column = col;
            move.rank = tmpvalue;
          }
        }
      }
    }
    return move;
  }

  int StudentAI::Minimax(int _color, const Board&  _board, int _lookAheadDepth)
  {
    if (_lookAheadDepth == 0 || _board.IsTerminalState())
    {
      return Evaluate(_board);
    }
    if (!_board.HasAnyValidMove(_color))
    {
      _color = OtherColor(_color);
    }
    if (_color == Board::Black)
    {
      int value = std::numeric_limits<int>::min();
      for (int row = 0; row < Board::Height; row++)
      {
        for (int col = 0; col < Board::Width; col++)
        {
          if (_board.IsValidMove(_color, row, col))
          {
            Board b = Board();
            b.Copy(_board);
            b.MakeMove(_color, row, col);
            int tmpvalue = Minimax(OtherColor(_color), b, _lookAheadDepth - 1);
            value = value > tmpvalue ? value : tmpvalue;
          }
        }
      }
      return value;
    }
    if (_color == Board::White)
    {
      int value = std::numeric_limits<int>::max();
      for (int row = 0; row < Board::Height; row++)
      {
        for (int col = 0; col < Board::Width; col++)
        {
          if (_board.IsValidMove(_color, row, col))
          {
            Board b = Board();
            b.Copy(_board);
            b.MakeMove(_color, row, col);
            int tmpvalue = Minimax(OtherColor(_color), b, _lookAheadDepth - 1);
            value = value < tmpvalue ? value : tmpvalue;
          }
        }
      }
      return value;
    }
    throw "Invalid Color";
  }

  int StudentAI::OtherColor(int _color)
  {
    if (_color == Board::Black)
    {
      return Board::White;
    }
      return Board::Black;
  }

  int StudentAI::Evaluate(const Board&  board)
  {
    int value = 0;
    if (board.IsTerminalState())
    {
      value += board.whiteCount > board.blackCount ? -10000 : 10000;
    }
    value += -1 * board.GetTile(0, 0) * 25;
    value += -1 * board.GetTile(Board::Height - 1, 0) * 25;
    value += -1 * board.GetTile(Board::Height - 1, Board::Width - 1) * 25;
    value += -1 * board.GetTile(0, Board::Width - 1) * 25;

    value += -1 * board.GetTile(1, 0) * -10;
    value += -1 * board.GetTile(1, 1) * -10;
    value += -1 * board.GetTile(0, 1) * -10;
    value += -1 * board.GetTile(Board::Height - 2, 0) * -10;
    value += -1 * board.GetTile(Board::Height - 2, 1) * -10;
    value += -1 * board.GetTile(Board::Height - 1, 1) * -10;
    value += -1 * board.GetTile(Board::Height - 2, Board::Width - 1) * -10;
    value += -1 * board.GetTile(Board::Height - 2, Board::Width - 2) * -10;
    value += -1 * board.GetTile(Board::Height - 1, Board::Width - 2) * -10;
    value += -1 * board.GetTile(1, Board::Width - 1) * -10;
    value += -1 * board.GetTile(1, Board::Width - 2) * -10;
    value += -1 * board.GetTile(0, Board::Width - 2) * -10;

    for (int row = 1; row < (Board::Height - 1); row++)
    {
      value += -1 * board.GetTile(row, 0) * 25;
      value += -1 * board.GetTile(row, Board::Width - 1) * 25;
    }
    for (int col = 1; col < (Board::Width - 1); col++)
    {
      value += -1 * board.GetTile(0, col) * 25;
      value += -1 * board.GetTile(Board::Height - 1, col) * 25;
    }

    int blackmoves = 0;
    int whitemoves = 0;

    int frontierblack = 0;
    int frontierwhite = 0;
    for (int row = 0; row < Board::Height; row++)
    {
      for (int col = 0; col < Board::Width; col++)
      {
        int piece = board.GetTile(row, col);
        if (piece == Board::Empty)
        {
          continue;
        }
        std::vector<int> x_offsets = {  0,  1, 1, 1, 0, -1, -1, -1 };
        std::vector<int> y_offsets = { -1, -1, 0, 1, 1,  1,  0, -1 };
        for(int i = 0; i < 8; i++)
        {
          if(!(col+x_offsets[i] > 0 && col+x_offsets[i] < Board::Width && row + y_offsets[i] > 0 && row + y_offsets[i] < Board::Height))
          {
            continue;
          }
          if(board.GetTile(row + y_offsets[i], col + x_offsets[i]) != Board::Empty)
          {
            continue;
          }
          bool found = false;
          if(piece == Board::Black)
          {
            frontierblack++;
            found = true;
          }
          if (piece == Board::White)
          {
            frontierwhite++;
            found = true;
          }
          if (found)
          {
            break;
          }
        }
      }
    }
    value += blackmoves * 1;
    value += whitemoves * -5;
    value += frontierblack * -1;
    value += frontierwhite * 1;


    return value;
  }

}
