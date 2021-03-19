#pragma once

//
// Modified: 2011/04/26, Derek Bliss (Full Sail University)
// Modified: 2019/12/20, Jeremiah Blanchard (University of Florida)
namespace GameAI::GamePlaying::Core
{
    /// <summary>
    /// Class for holding a look ahead move and rank.
    /// </summary>
    class ComputerMove
    {
        public:
            // Defines a structure for holding a look ahead move and rank.
            int row;
            int column;
            int rank;

            ComputerMove(int row, int column):row(row),column(column),rank(0){}
    };
}
