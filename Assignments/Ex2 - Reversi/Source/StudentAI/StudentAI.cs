using GameAI.GamePlaying.Core;
using System.Collections;

namespace GameAI.GamePlaying
{
  public class StudentAI : Behavior
  {
    public ComputerMove Run(int _color, Board _board, int _lookAheadDepth)
    {
      if(_color != Board.Black)
      {
        throw new System.Exception("What");
      }
      ComputerMove move = new ComputerMove(-1,-1);
      move.rank = int.MinValue;
      for (int row = 0; row < Board.Height; row++)
      {
        for (int col = 0; col < Board.Width; col++)
        {
          if (_board.IsValidMove(_color, row, col))
          {
            Board b = new Board();
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

    private int Minimax(int _color, Board _board, int _lookAheadDepth)
    {
      if (_lookAheadDepth == 0 || _board.IsTerminalState())
      {
        return Evaluate(_board);
      }
      if (!_board.HasAnyValidMove(_color))
      {
        _color = OtherColor(_color);
      }
      if (_color == Board.Black)
      {
        int value = int.MinValue;
        for (int row = 0; row < Board.Height; row++)
        {
          for (int col = 0; col < Board.Width; col++)
          {
            if (_board.IsValidMove(_color, row, col))
            {
              Board b = new Board();
              b.Copy(_board);
              b.MakeMove(_color, row, col);
              int tmpvalue = Minimax(OtherColor(_color), b, _lookAheadDepth - 1);
              value = value > tmpvalue ? value : tmpvalue;
            }
          }
        }
        return value;
      }
      if (_color == Board.White)
      {
        int value = int.MaxValue;
        for (int row = 0; row < Board.Height; row++)
        {
          for (int col = 0; col < Board.Width; col++)
          {
            if (_board.IsValidMove(_color, row, col))
            {
              Board b = new Board();
              b.Copy(_board);
              b.MakeMove(_color, row, col);
              int tmpvalue = Minimax(OtherColor(_color), b, _lookAheadDepth - 1);
              value = value < tmpvalue ? value : tmpvalue;
            }
          }
        }
        return value;
      }
      throw new System.Exception("Invalid Color");
    }

    private int OtherColor(int _color)
    {
      if (_color == Board.Black)
      {
        return Board.White;
      }
        return Board.Black;
    }

    private int Evaluate(Board board)
    {
      int value = 0;
      if (board.IsTerminalState())
      {
        value += board.WhiteCount > board.BlackCount ? -10000 : 10000;
      }
      value += -1 * board.GetTile(0, 0) * 25;
      value += -1 * board.GetTile(Board.Height - 1, 0) * 25;
      value += -1 * board.GetTile(Board.Height - 1, Board.Width - 1) * 25;
      value += -1 * board.GetTile(0, Board.Width - 1) * 25;

      value += -1 * board.GetTile(1, 0) * -10;
      value += -1 * board.GetTile(1, 1) * -10;
      value += -1 * board.GetTile(0, 1) * -10;
      value += -1 * board.GetTile(Board.Height - 2, 0) * -10;
      value += -1 * board.GetTile(Board.Height - 2, 1) * -10;
      value += -1 * board.GetTile(Board.Height - 1, 1) * -10;
      value += -1 * board.GetTile(Board.Height - 2, Board.Width - 1) * -10;
      value += -1 * board.GetTile(Board.Height - 2, Board.Width - 2) * -10;
      value += -1 * board.GetTile(Board.Height - 1, Board.Width - 2) * -10;
      value += -1 * board.GetTile(1, Board.Width - 1) * -10;
      value += -1 * board.GetTile(1, Board.Width - 2) * -10;
      value += -1 * board.GetTile(0, Board.Width - 2) * -10;

      for (int row = 1; row < (Board.Height - 1); row++)
      {
        value += -1 * board.GetTile(row, 0) * 25;
        value += -1 * board.GetTile(row, Board.Width - 1) * 25;
      }
      for (int col = 1; col < (Board.Width - 1); col++)
      {
        value += -1 * board.GetTile(0, col) * 25;
        value += -1 * board.GetTile(Board.Height - 1, col) * 25;
      }

      int blackmoves = 0;
      int whitemoves = 0;

      int frontierblack = 0;
      int frontierwhite = 0;
      for (int row = 0; row < Board.Height; row++)
      {
        for (int col = 0; col < Board.Width; col++)
        {
          if (board.IsValidMove(Board.Black, row, col))
          {
            blackmoves++;
          }
          if (board.IsValidMove(Board.White, row, col))
          {
            whitemoves++;
          }
          int piece = board.GetTile(row, col);
          if (piece == Board.Empty)
          {
            continue;
          }
          int[] x_offsets = {  0,  1, 1, 1, 0, -1, -1, -1 };
          int[] y_offsets = { -1, -1, 0, 1, 1,  1,  0, -1 };
          for(int i = 0; i < 8; i++)
          {
            if(!(col+x_offsets[i] > 0 && col+x_offsets[i] < Board.Width && row + y_offsets[i] > 0 && row + y_offsets[i] < Board.Height))
            {
              continue;
            }
            if(board.GetTile(row + y_offsets[i], col + x_offsets[i]) != Board.Empty)
            {
              continue;
            }
            bool found = false;
            if(piece == Board.Black)
            {
              frontierblack++;
              found = true;
            }
            if (piece == Board.White)
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
}
