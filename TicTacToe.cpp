#include "TicTacToe.h"

int TicTacToe::boardSizeInt;
char TicTacToe::hostSymbol;
char TicTacToe::clientSymbol;
std::vector<std::vector<char>> TicTacToe::ticTacToeBoard;
int TicTacToe::previousMove[] = {-1, -1};

//Constructor that sets board size to 3.
TicTacToe::TicTacToe()
{
	int boardSize = 3;

	TicTacToe::createTicTacToeBoard(boardSize);
	TicTacToe::hostSymbol = 'X';
	TicTacToe::clientSymbol = 'O'; 
}

//Constructor that sets board size based on the passed in int.
TicTacToe::TicTacToe(int& boardSize)
{
	TicTacToe::createTicTacToeBoard(boardSize);
	TicTacToe::hostSymbol = 'X';
	TicTacToe::clientSymbol = 'O'; 
}

/*Set boardSizeInt to the passed in reference boardSizeInt.*/
void TicTacToe::setBoardSize(int& newBoardSizeInt)
{
	TicTacToe::boardSizeInt = newBoardSizeInt;
}

/*Returns a reference to the boardHeightInt as an int.*/
int& TicTacToe::getBoardSize()
{
	return boardSizeInt;
}

/*Retruns a 2-D vector of type char. Creates a Tic-Tac-Toe board with a height and width of the passed in references.*/
void TicTacToe::createTicTacToeBoard(int& boardSizeInt)
{
	TicTacToe::setBoardSize(boardSizeInt);
	std::vector<std::vector<char>> newTicTacToeBoard;

	for (int row = 0; row < boardSizeInt; ++row)
	{
		std::vector<char> rowVectorChar;
	
		for (int col = 0; col < boardSizeInt; ++col)
		{
			rowVectorChar.push_back(' ');
		}
		
		newTicTacToeBoard.push_back(rowVectorChar);
	}
	
	ticTacToeBoard = newTicTacToeBoard;
}

/*Returns a reference to the current state of the Tic-Tac-Toe board.*/
std::vector<std::vector<char>>& TicTacToe::getTicTacToeBoard()
{
	return ticTacToeBoard;
}

/*Prints out the current state of the Tic-Tac-Toe board.*/
void TicTacToe::displayTicTacToeBoard()
{
	for (int row = 0; row < boardSizeInt; ++row)
	{
		for (int col = 0; col < boardSizeInt; ++col)
		{
			if (col < boardSizeInt - 1)
			{
				std::cout << " " << ticTacToeBoard[row][col] << " |";
			}
			else
			{
				std::cout << " " << ticTacToeBoard[row][col] << " ";
			}
		}
		
		std::cout << std::endl;
		
		if (row < boardSizeInt - 1)
		{
			for (int col = 0; col < boardSizeInt; ++col)
			{
				if (col < boardSizeInt - 1)
				{
					std::cout << "———+";
				}
				else
				{
					std::cout << "———" << std::endl;
				}
			}
		}
	}
}

//Checks if the previous move produced a winner. Returns true if a winner is present.
bool TicTacToe::checkWinner()
{
	if (TicTacToe::previousMove[0] < 0 || TicTacToe::previousMove[1] < 0) return false;

	char& playerSymbol = TicTacToe::ticTacToeBoard[TicTacToe::previousMove[0]][TicTacToe::previousMove[1]];
	
	int count = 1;

	//Check for horizontal win.
	for (int rowRight = TicTacToe::previousMove[0] + 1, rowLeft = TicTacToe::previousMove[0] - 1; (rowRight < TicTacToe::boardSizeInt || rowLeft >= 0);)
	{
		if (rowRight < TicTacToe::boardSizeInt)
		{
			if (TicTacToe::ticTacToeBoard[rowRight][TicTacToe::previousMove[1]] == playerSymbol)
			{
				count++;
				rowRight++;
			}
			else break;
		}
		
		if (rowLeft >= 0)
		{
			if (TicTacToe::ticTacToeBoard[rowLeft][TicTacToe::previousMove[1]] == playerSymbol)
			{
				count++;
				rowLeft--;
			}
			else break;
		}
	}
	
	if (count == TicTacToe::boardSizeInt) return true;
	
	count = 1;
	
	//Check for vertical win.
	for (int colRight = TicTacToe::previousMove[1] + 1, colLeft = TicTacToe::previousMove[1] - 1; (colRight < TicTacToe::boardSizeInt || colLeft >= 0);)
	{
		if (colRight < TicTacToe::boardSizeInt)
		{
			if (TicTacToe::ticTacToeBoard[TicTacToe::previousMove[0]][colRight] == playerSymbol)
			{
				count++;
				colRight++;
			}
			else break;
		}
		
		if (colLeft >= 0)
		{
			if (TicTacToe::ticTacToeBoard[TicTacToe::previousMove[0]][colLeft] == playerSymbol)
			{
				count++;
				colLeft--;
			}
			else break;
		}
	}
	
	if (count == TicTacToe::boardSizeInt) return true;
	
	count = 0;
	
	//Check for top-left to bottom-right win.
	if (previousMove[0] == previousMove[1])
	{
		for (int rowCol = TicTacToe::boardSizeInt - 1; rowCol >= 0; --rowCol)
		{
			if (TicTacToe::ticTacToeBoard[rowCol][rowCol] == playerSymbol)
			{
				count++;
			}
			else break;
		}
	}
	
	if (count == TicTacToe::boardSizeInt) return true;
	
	count = 0;
	
	//Check for bottom-right to top-right win.
	if (previousMove[0] + previousMove[1] == TicTacToe::boardSizeInt - 1)
	{
		for (int iterator = 0; iterator < TicTacToe::boardSizeInt; ++iterator)
		{
			if (TicTacToe::ticTacToeBoard[iterator][(TicTacToe::boardSizeInt - 1) - iterator] == playerSymbol) count++;
			else break;
			
		}
	}
	
	if (count == TicTacToe::boardSizeInt) return true;
	else return false;
}

//Checks if a draw has occurred. Returns true if a draw.
bool TicTacToe::checkDraw()
{
	if (TicTacToe::checkWinner()) return false;

	for (int row = 0; row < TicTacToe::boardSizeInt; ++row)
	{
		for (int col = 0; col < TicTacToe::boardSizeInt; ++col)
		{
			if (TicTacToe::ticTacToeBoard[row][col] == ' ') return false;
		}
	}
	
	return true;
}

//Registers a play based on the passed in coordinates and which player made the play. Returns false if the play failed.
bool TicTacToe::makePlay(int& inRow, int& inCol, bool hostPlay)
{
	if (inRow < 1 || inCol < 1) return false;

	int row = inRow - 1;
	int col = inCol - 1;

	if (TicTacToe::ticTacToeBoard[row][col] != ' ') return false;

	try
	{
		if (hostPlay)
		{
			TicTacToe::ticTacToeBoard[row][col] = TicTacToe::hostSymbol;
		}
		else
		{
			TicTacToe::ticTacToeBoard[row][col] = TicTacToe::clientSymbol;
		}
		
		TicTacToe::previousMove[0] = row;
		TicTacToe::previousMove[1] = col;
		return true;
	}
	catch(...)
	{
		std::cout << "Severe error. Exiting...\n";
		std::exit(2);
	}
	
	return false;
}
