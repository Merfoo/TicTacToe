#include <iostream>

#include "TicTacToe.h"

int main()
{
	std::cout << "Hello world!" << std::endl;

	TicTacToe game('X', 'O', ' ');
	game.enableAi(true, false);
	int turns = 0;

	while (turns < 9)
	{
		if (!game.isAiTurn())
		{
			std::cout << "Your turn!\n";
			int x;
			int y;

			std::cin >> y >> x;
			std::cout << x << " : " << y << std::endl;
			game.insertMove(x, y);
		}

		else
			game.insertAiMove();

		std::cout << std::endl;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
				std::cout << game.getSpot(i, j) << "|\t";

			std::cout << std::endl;
		}

		char winner = game.getWinner();

		if(winner != game.getEmptySpot())
			std::cout << "WINNER! " << winner << std::endl;
	}

	std::cin.get();

	return 0;
}