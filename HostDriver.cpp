#include "TicTacToe.h"
#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <iostream>
#include <climits>

#define INDICES 5

int main(int argc, char** argv)
{
	int boardSize;

	//If no parameter then the default board size will be 3.
	if (argc == 1)
	{
		boardSize = 3;
	}
	//If an argument is passed in then it will be checked if an int and if 3 or greater. Otherwise throw an error.
	else if (argc == 2)
	{
		try
		{
			boardSize = std::stoi(argv[1]);
			
			if (boardSize < 3) throw std::invalid_argument("Number less than three.");
		}
		catch(...)
		{
			std::cout << "\033[;31mPassed in value must be a positive number of 3 or greater.\033[0m\n";
			return 1;
		}
	}
	//If more than one parameter is given then give an error before exiting.
	else
	{
		std::cout << "\033[;31mPass in the size of the tic-tac-toe board as a parameter.\033[0m\n";
		return 1;
	}
	
	//Generate key
	key_t key = ftok("tictactoe", 34);
	
	//Generate and save shget id.
	int shmid = shmget(key, sizeof(int) * INDICES, 0666|IPC_CREAT);
	
	if (shmid < 0)
	{
		perror("shmget");
		exit(1);
	}
	
	//Attaching to shared memory.
	int* sharedMemory = (int*) shmat(shmid, (void*)0, 0);
	
	//Set all shared memory indices to the smallest possible int value.
	for (int x = 0; x < INDICES; ++x)
	{
		sharedMemory[x] = INT_MIN;
	}
	
	//Host has their turn first.
	sharedMemory[0] = 0;
	
	//Saves board size.
	sharedMemory[4] = boardSize;
	
	//Creates the tic-tac-toe board and game.
	TicTacToe game = TicTacToe(boardSize);
	
	std::cout << std::endl;
	
	//Play tic-tac-toe
	while(1)
	{
		sharedMemory[1] = INT_MIN;
		sharedMemory[2] = INT_MIN;
	
		if (sharedMemory[0] == 1)
		{
			while(1)
			{
				//Makes client's play.
				if (game.makePlay(sharedMemory[1], sharedMemory[2], false) == false) 
				{
					sharedMemory[3] = -2; 
					
					while(sharedMemory[3] == -2);
					
					continue;
				}
			
				//Checks for win and draw.
				if (game.checkWinner()) sharedMemory[3] = 1;
				else if (game.checkDraw()) sharedMemory[3] = 2;
				
				
				sharedMemory[3] = -3;
				sharedMemory[0] = 0;
				break;
			}
		}
	
		//Displays the tic-tac-toe board.
		game.displayTicTacToeBoard();
		std::cout << std::endl;
		
		//If turn is not 0 then wait.
		while(sharedMemory[0] != 0);
		
		while(1)
		{
			if (sharedMemory[3] == 1 || sharedMemory[3] == 2) break;
		
			int row = -1;
			int col = -1;
		
			//Retrieves host's row coordinate.
			while (row < 1 || row > boardSize)
			{
				std::string rowStr;
			
				std::cout << "Enter a row: ";
			
				try
				{
					std::cin >> rowStr;
					
					row = stoi(rowStr);
					
					if (row < 1) std::cout << "Enter a row coordinate greater than or equal to 1.\n";
					else if (row > sharedMemory[4]) std::cout << "Enter a row coordinate less than or equal to " << sharedMemory[4] << ".\n";
				}
				catch(...)
				{
					std::cout << "ENTER A NUMBER!\n";
					row = -1;
				}
			}
		
			//Retrieves host's column coordinate.
			while (col < 1 || col > boardSize)
			{
				std::string colStr;
			
				std::cout << "Enter a column: ";
			
				try
				{
					std::cin >> colStr;
					
					col = stoi(colStr);
					
					if (col < 1) std::cout << "Enter a column coordinate greater than or equal to 1.\n";
					else if (col > sharedMemory[4]) std::cout << "Enter a column coordinate less than or equal to " << sharedMemory[4] << ".\n";
				}
				catch(...)
				{
					std::cout << "ENTER A NUMBER!\n";
					col = -1;
				}
			}
			
			//Checks if the play can be made. If not then have user enter coordinates again.
			if (game.makePlay(row, col, true) == false)
			{
				std::cout << "Invalid coordinates. Enter coordinates to a free space.\n";
				continue;
			}
			
			//Checks for win and draw.
			if (game.checkWinner()) sharedMemory[3] = 0;
			else if (game.checkDraw()) sharedMemory[3] = 2;
			
			//Displays the tic-tac-toe board.
			std::cout << std::endl;
			game.displayTicTacToeBoard();
			std::cout << std::endl;
			
			std::cout << "Waiting for player O to make a play...\n\n";
			
			break;
		}
		
		sharedMemory[0] = 1;
		
		//Display win if host won.
		if (sharedMemory[3] == 0)
		{
			std::cout << "\nYOU WON!!!\n";
			
			//Wait for client to acknowledge.
			while (sharedMemory[3] != -1);
			
			break;
		}
		//Display loss if host lost.
		else if (sharedMemory[3] == 1)
		{
			std::cout << "\nYOU LOST!!!\n";
			
			//Wait for client to acknowledge.
			while (sharedMemory[3] != -1);
			
			break;
		}
		//Display draw if draw.
		else if (sharedMemory[3] == 2)
		{
			std::cout << "\nIT'S A DRAW!!!\n";
			
			//Wait for client to acknowledge.
			while (sharedMemory[3] != -1);
			
			break;
		}
	}
	
	//Detach from shared memory.
	shmdt(sharedMemory);
	
	//Destroy shared memory.
	shmctl(shmid, IPC_RMID, NULL);

	return 0;
}
