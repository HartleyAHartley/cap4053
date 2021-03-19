#pragma once

//
// Modified: 2011/04/26, Derek Bliss (Full Sail University)
// Modified: 2019/12/20, Jeremiah Blanchard (University of Florida)
namespace GameAI::GamePlaying::Core
{
    /// <summary>
    /// Class representing the reversi game board.
    /// </summary>
    class Board
    {
        public:
            // These constants represent the contents of a board square.
            static const int Black = -1;
            static const int Empty =  0;
            static const int White =  1;
            static const int Width = 8;
            static const int Height = 8;

            //counts.
            int blackCount;
            int whiteCount;
            int emptyCount;
            inline int Score() const
            {
                return whiteCount - blackCount;
            }
            inline int& operator[](int position)
            {
                return squares[position];
            }
            inline int operator[](int position) const
            {
                return squares[position];
            }


             //
            // Creates a new, empty Board object.
            //
            Board();

            //
            // Creates a new Board object by copying an existing one.
            //
            Board(const Board& board);

            //
            // Sets this Board as a copy of board.
            //
            void Copy(const Board& board);

            //
            // Sets a board with the initial game set-up.
            //
            void SetForNewGame();

            //
            // Returns the contents of a given board square.
            //
            inline int GetTile(int row, int col) const
            {
                return squares[row*Width+col];
            }

            //
            // Places a disc for the player on the board and flips any outflanked
            // opponents.
            // Note: For performance reasons, it does NOT check that the move is
            // valid.
            //
            void MakeMove(int color, int row, int col);

            //
            // Determines if the player can make any valid move on the board.
            //
            bool HasAnyValidMove(int color) const;

            //
            // Determines if the game is over (ie, no valid moves for either plaer.)
            //
            inline bool IsTerminalState() const
            {
                return (!HasAnyValidMove(Black) && !HasAnyValidMove(White));
            }

            //
            // Determines if a specific move is valid for the player.
            //
            bool IsValidMove(int color, int row, int col) const;

            //
            // Returns the number of valid moves a player can make on the board.
            //
            int GetValidMoveCount(int color) const;

        private:

            // This two-dimensional array represents the squares on the board.
            int* squares;

            //
            // Given a player move and a specific direction, determines if any
            // opponent discs will be outflanked.
            // Note: For performance reasons the direction values are NOT checked
            // for validity (dr and dc may be one of -1, 0 or 1 but both should
            // not be zero).
            //
            bool IsOutflanking(int color, int row, int col, int dr, int dc) const;

            //
            // Updates the board counts and safe disc map.
            // Note: MUST be called after any changes to the board contents.
            //
            void UpdateCounts();
    };
}
