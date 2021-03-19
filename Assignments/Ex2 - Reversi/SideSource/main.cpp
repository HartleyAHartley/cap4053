#include <iostream>
#include <vector>

#include "board.h"
#include "minimaxexample.h"
#include "studentai.h"

using namespace GameAI::GamePlaying;
using Core::Board,Core::ComputerMove,ExampleAI::MinimaxExample;

int main(){
  //StudentAI studentAI;
  MinimaxExample studentAI;
  MinimaxExample exampleAI;
  bool black = true;

  std::vector<int> difficulties = {0,1,3};

  for(int blackdif = 0; blackdif < 3; blackdif++){
    for(int whitedif = 0; whitedif < 3; whitedif++){
      Board board;
      board.SetForNewGame();
      while(!board.IsTerminalState()){
        ComputerMove move(-1,-1);
        if(!board.HasAnyValidMove(black ? Board::Black : Board::White)){
          black = !black;
        }
        if(black){
          move = studentAI.Run(Board::Black,board,difficulties[blackdif]);
        }else{
          move = exampleAI.Run(Board::White,board,difficulties[whitedif]);
        }
        if(!board.IsValidMove(black ? Board::Black : Board::White,move.row,move.column)){
          throw "Invalid Move";
        }
        // std::cout << (black ? "Black " : "White ") << " Move: " << move.column << ", " << move.row << std::endl;
        board.MakeMove(black ? Board::Black : Board::White,move.row,move.column);
        black = !black;
      }
      std::cout << "difficulties: black(" << difficulties[blackdif] << ") white(" << difficulties[whitedif] << ")" << std::endl;
      if(board.blackCount > board.whiteCount){
        std::cout << "Black won: " << board.blackCount << "-" << board.whiteCount << std::endl;
        continue;
      }
      if(board.whiteCount > board.blackCount){
        std::cout << "White won: " << board.blackCount << "-" << board.whiteCount << std::endl;
        continue;
      }
      std::cout << "Draw: " << board.blackCount << "-" << board.whiteCount << std::endl;
    }
  }
  return 0;
}
