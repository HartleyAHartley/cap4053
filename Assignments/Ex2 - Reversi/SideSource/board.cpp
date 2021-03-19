#include "board.h"
//
// Modified: 2011/04/26, Derek Bliss (Full Sail University)
// Modified: 2019/12/20, Jeremiah Blanchard (University of Florida)
namespace GameAI::GamePlaying::Core
{

    //
    // Creates a new, empty Board object.
    //
    Board::Board()
    {
        // Create the squares and safe disc map.
        squares = new int[Height*Width];

        // Clear the board and map.
        int i, j;
        for (i = 0; i < Height; i++)
            for (j = 0; j < Width; j++)
            {
                squares[i*Width+j] = Empty;
            }

        // Update the counts.
        UpdateCounts();
    }

    //
    // Creates a new Board object by copying an existing one.
    //
    Board::Board(const Board& board)
    {
        // Create the squares and map.
        squares = new int[Height*Width];

        // Copy the given board.
        int i, j;
        for (i = 0; i < Height; i++)
            for (j = 0; j < Width; j++)
            {
                squares[i*Width+j] = board.squares[i*Width+j];
            }

        // Copy the counts.
        blackCount = board.blackCount;
        whiteCount = board.whiteCount;
        emptyCount = board.emptyCount;
    }

    //
    // Sets this Board as a copy of board.
    //
    void Board::Copy(const Board& board)
    {
        // Copy the given board.
        int i, j;
        for (i = 0; i < Height; i++)
            for (j = 0; j < Width; j++)
            {
                squares[i*Width+j] = board.squares[i*Width+j];
            }

        // Copy the counts.
        blackCount = board.blackCount;
        whiteCount = board.whiteCount;
        emptyCount = board.emptyCount;
    }

    //
    // Sets a board with the initial game set-up.
    //
    void Board::SetForNewGame()
    {
        // Clear the board.
        int i, j;
        for (i = 0; i < 8; i++)
            for (j = 0; j < 8; j++)
            {
                squares[i*Width+j] = Empty;
            }

        // Set two black and two white discs in the center.
        squares[3*Width+3] = White;
        squares[3*Width+4] = Black;
        squares[4*Width+3] = Black;
        squares[4*Width+4] = White;

        // Update the counts.
        UpdateCounts();
    }

    //
    // Places a disc for the player on the board and flips any outflanked
    // opponents.
    // Note: For performance reasons, it does NOT check that the move is
    // valid.
    //
    void Board::MakeMove(int color, int row, int col)
    {
        // Set the disc on the square.
        squares[row*Width+col] = color;

        // Flip any flanked opponents.
        int dr, dc;
        int r, c;
        for (dr = -1; dr <= 1; dr++)
            for (dc = -1; dc <= 1; dc++)
                // Are there any outflanked opponents?
                if (!(dr == 0 && dc == 0) && IsOutflanking(color, row, col, dr, dc))
                {
                    r = row + dr;
                    c = col + dc;
                    // Flip 'em.
                    while (squares[r*Width+c] == -color)
                    {
                        squares[r*Width+c] = color;
                        r += dr;
                        c += dc;
                    }
                }

        // Update the counts.
        UpdateCounts();
    }

    //
    // Determines if the player can make any valid move on the board.
    //
    bool Board::HasAnyValidMove(int color) const
    {
        // Check all board positions for a valid move.
        int r, c;
        for (r = 0; r < 8; r++)
            for (c = 0; c < 8; c++)
                if (IsValidMove(color, r, c))
                    return true;

        // None found.
        return false;
    }

    //
    // Determines if a specific move is valid for the player.
    //
    bool Board::IsValidMove(int color, int row, int col) const
    {   
        if(row < 0 || row > Height || col < 0 || col > Width){
            return false;
        }
        // The square must be empty.
        if (squares[row*Width+col] != Empty)
            return false;

        // Must be able to flip at least one opponent disc.
        int dr, dc;
        for (dr = -1; dr <= 1; dr++)
            for (dc = -1; dc <= 1; dc++)
                if (!(dr == 0 && dc == 0) && IsOutflanking(color, row, col, dr, dc))
                    return true;

        // No opponents could be flipped.
        return false;
    }

    //
    // Returns the number of valid moves a player can make on the board.
    //
    int Board::GetValidMoveCount(int color) const
    {
        int n = 0;

        // Check all board positions.
        int i, j;
        for (i = 0; i < Height; i++)
            for (j = 0; j < Width; j++)
                // If the move is valid for the color, bump the count.
                if (IsValidMove(color, i, j))
                    n++;
        return n;
    }

    //
    // Given a player move and a specific direction, determines if any
    // opponent discs will be outflanked.
    // Note: For performance reasons the direction values are NOT checked
    // for validity (dr and dc may be one of -1, 0 or 1 but both should
    // not be zero).
    //
    bool Board::IsOutflanking(int color, int row, int col, int dr, int dc) const
    {
        // Move in the given direction as long as we stay on the board and
        // land on a disc of the opposite color.
        int r = row + dr;
        int c = col + dc;
        while (r >= 0 && r < Height && c >= 0 && c < Width && squares[r*Width+c] == -color)
        {
            r += dr;
            c += dc;
        }

        // If we ran off the board, only moved one space or didn't land on
        // a disc of the same color, return false.
        if (r < 0 || r > (Height - 1) || c < 0 || c > (Width - 1) || (r - dr == row && c - dc == col) || squares[r*Width+c] != color)
            return false;

        // Otherwise, return true;
        return true;
    }

    //
    // Updates the board counts and safe disc map.
    // Note: MUST be called after any changes to the board contents.
    //
    void Board::UpdateCounts()
    {
        // Reset all counts.
        blackCount = 0;
        whiteCount = 0;
        emptyCount = 0;

        int i, j;

        // Tally the counts.
        for (i = 0; i < Height; i++)
            for (j = 0; j < Width; j++)
            {
                // Update the counts.
                if (squares[i*Width+j] == Black)
                {
                    blackCount++;
                }
                else if (squares[i*Width+j] == White)
                {
                    whiteCount++;
                }
                else
                    emptyCount++;
            }
    }
}
