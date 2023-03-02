#include "TicTacToe.h"
#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <iostream>
#include <climits>

#define INDICES 5

int main(int argc, char** argv)
{
	if (argc != 1 || argv[1] != NULL) 
	{
		std::cout << "\033[;31mNot supported for any passed in parameter.\033[0m\n";
		return 1;
	}
	
	//Generate key
	key_t key = ftok("tictactoe", 34);
	
	//Generate and save shget id.
	int shmid = shmget(key, sizeof(int) * INDICES, 0666|IPC_CREAT);
	
	//Attaching to shared memory.
	int* sharedMemory = (int*) shmat(shmid, (void*)0, 0);
	
	//Play tic-tac-toe
	while(1)
	{
		std::cout << "Waiting for player X to make a play...\n\n";
	
		//If game state is not 1 then wait.
		while(sharedMemory[0] != 1);

		while(sharedMemory[3] != -3)
		{ 
			if (sharedMemory[3] == 0 || sharedMemory[3] == 1 || sharedMemory[3] == 2) break;
			
			int row = -1;
			int col = -1;
		
			//Retrieves host's row coordinate.
			while (row < 1 || row > sharedMemory[4])
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
			while (col < 1 || col > sharedMemory[4])
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
			
			sharedMemory[1] = row;
			sharedMemory[2] = col;
			
			sharedMemory[3] = -4;
			
			while(sharedMemory[3] == -4);
		}
		
		//Display win if host won.
		if (sharedMemory[3] == 0)
		{
			std::cout << "\nYOU LOST!!!\n";
			
			//Client acknowledgement.
			sharedMemory[3] = -1;
			
			break;
		}
		//Display loss if host lost.
		else if (sharedMemory[3] == 1)
		{
			std::cout << "\nYOU WON!!!\n";
			
			//Client acknowledgement.
			sharedMemory[3] = -1;
			
			break;
		}
		//Display draw if draw.
		else if (sharedMemory[3] == 2)
		{
			std::cout << "\nIT'S A DRAW!!!\n";
			
			//Client acknowledgement.
			sharedMemory[3] = -1;
			
			break;
		}
	}
	
	//Detach from shared memory.
	shmdt(sharedMemory);
	
	return 0;
}
