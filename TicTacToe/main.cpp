#include <iostream>
#include <string>

#include "TicTacToe.h"

void playLocally(TicTacToe game)
{
	bool catsGame = true;

	while (game.getMovesMade() < TicTacToe::MAX_MOVES)
	{
		if ((game.aiEnabled() && !game.isAiTurn()) || !game.aiEnabled())
		{
			bool invalidInput = false;
			int x = 0;
			int y = 0;

			while (true)
			{
				game.draw();

				if (invalidInput)
					std::cout << "Previous input invalid!\n";

				std::cout << " Your turn (" << (game.isPlayerOneTurn() ? game.getPlayerOne() : game.getPlayerTwo()) << ") \n";

				invalidInput = true;
				std::string input;
				std::getline(std::cin, input);

				if (input.length() >= 3)
				{
					x = (int(input[2]) - 48) - 1;
					y = (int(input[0]) - 48) - 1;

					if (x >= 0 && x < 3 && y >= 0 && y < 3)
						if (game.getSpot(x, y) == game.getEmptySpot())
							invalidInput = false;
				}

				if (!invalidInput)
					break;
			}

			game.insertMove(x, y);
		}

		else
			game.insertAiMove();

		char winner = game.getWinner();

		if (winner != game.getEmptySpot())
		{
			game.draw();
			std::cout << " WINNER! " << winner << std::endl;
			catsGame = false;
			break;
		}
	}

	if (catsGame)
	{
		game.draw();
		std::cout << "\n CATS GAME! \n" << std::endl;
	}
}

int main()
{
	const char playVsOnline = 'o';
	const char playVsFriend = 'p';
	const char playVsAi     = 'a';
	
	bool invalidInput = false;

	while (true)
	{
		if (invalidInput)
		{
			std::cout << "INVALLID INPUT" << std::endl;
			invalidInput = false;
		}

		TicTacToe game('X', 'O', ' ');
		std::cout << "Play with a friend online?  [" << playVsOnline << "]" << std::endl;
		std::cout << "Play locally with a friend? [" << playVsFriend << "]" << std::endl;
		std::cout << "Play locally against AI?    [" << playVsAi << "]" << std::endl;

		std::string input;
		std::getline(std::cin, input);

		if (input.length() > 0)
		{
			const char in = input[0];

			if (in == playVsOnline)
				return 0;

			else if (in == playVsFriend)
				playLocally(game);

			else if (in == playVsAi)
			{
				std::cout << "Be player 1? [y]" << std::endl;

				std::string enableAi;
				std::getline(std::cin, enableAi);

				if (enableAi.length() > 0 && enableAi[0] == 'y')
					game.enableAi(true);

				else
					game.enableAi(false);

				playLocally(game);
			}

			else
				invalidInput = true;
		}

		else
			invalidInput = true;
	}

	std::cin.get();

	return 0;
}