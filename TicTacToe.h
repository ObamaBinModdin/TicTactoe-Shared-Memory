#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <spawn.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

class TicTacToe
{
	public:
		TicTacToe();
		TicTacToe(int& boardSize);
			
		static void setBoardSize(int& newBoardSizeInt);
		static int& getBoardSize();
		static void createTicTacToeBoard(int& boardSizeInt);
		static std::vector<std::vector<char>>& getTicTacToeBoard();
		static void displayTicTacToeBoard();
		static void updateTicTacToeBoard(std::vector<std::vector<char>>& ticTacToeBoard);
		static void updateTicTacToeBoard(int& newBoardHeightInt, int& newBoardWidthInt, char& playerChar);
		static bool checkWinner();
		static bool checkDraw();
		static bool makePlay(int& row, int& col, bool hostPlay);
		
	private:
		static int boardSizeInt;
		static char hostSymbol;
		static char clientSymbol;
		static std::vector<std::vector<char>> ticTacToeBoard;
		static int previousMove[2];
};
