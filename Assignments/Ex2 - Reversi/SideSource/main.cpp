#include <iostream>
#include <vector>
#include <string>

#include "board.h"
#include "minimaxexample.h"
#include "studentai.h"

using namespace GameAI::GamePlaying;
using Core::Board,Core::ComputerMove,ExampleAI::MinimaxExample;

int main(){
  StudentAI studentAI;
  // MinimaxExample studentAI;
  MinimaxExample exampleAI;
  Board board;

  const std::string alpha = "ABCDEFGH";

  std::vector<int> difficulties = {0,1,3};
  std::vector<std::string> match = {"8-56","6-58","3-58","56-8","33-31","19-45","50-14","55-8","39-25"};


  for(int blackdif = 0; blackdif < difficulties.size(); blackdif++){
    for(int whitedif = 0; whitedif < difficulties.size(); whitedif++){
      bool black = true;
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
        // std::cout << (black ? "Black " : "White ") << alpha[move.column] << move.row+1 << std::endl;
        board.MakeMove(black ? Board::Black : Board::White,move.row,move.column);
        black = !black;
      }
      std::cout << "difficulties: black(" << difficulties[blackdif] << ") white(" << difficulties[whitedif] << "): ";
      if(board.blackCount > board.whiteCount){
        std::cout << "Black won: " << board.blackCount << "-" << board.whiteCount << "\t" << match[blackdif*3+whitedif] << std::endl;
        continue;
      }
      if(board.whiteCount > board.blackCount){
        std::cout << "White won: " << board.blackCount << "-" << board.whiteCount << "\t" << match[blackdif*3+whitedif] << std::endl;
        continue;
      }
      std::cout << "Draw: " << board.blackCount << "-" << board.whiteCount << std::endl;
    }
  }
  return 0;
}
