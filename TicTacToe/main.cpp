#include <iostream>
#include <string>

#include "TicTacToe.h"
#include "Address.h"
#include "Socket.h"

const int socketPort = 5000;

// TODO: Add timeouts during waiting for data from the other person if they want to continue waiting or just exit
void playOnline(TicTacToe game, const Address dest, Socket socket, const bool asPlayerOne)
{
	bool catsGame = true;

	while (game.getMovesMade() < TicTacToe::maxMoves)
	{
		bool invalidInput = false;
		int x = 0;
		int y = 0;

		if (game.isPlayerOneTurn() == asPlayerOne)
		{
			while (true)
			{
				game.draw();

				if (invalidInput)
					std::cout << "Previous input invalid!\n";

				std::cout << " Your turn (" << (asPlayerOne ? game.getPlayerOne() : game.getPlayerTwo()) << ")\n";

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
				{
					const char data[] = {x, y};

					socket.send(dest, data, sizeof(data));
					break;
				}
			}
		}

		while (true)
		{
			Address from;
			char data[2];

			if (socket.recieve(from, data, sizeof(data)) > 0)
			{
				x = data[0];
				y = data[1];
				break;
			}
		}

		game.insertMove(x, y);
	}
}

void playLocally(TicTacToe game)
{
	bool catsGame = true;

	while (game.getMovesMade() < TicTacToe::maxMoves)
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
	Socket::initializeSockets();

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
		std::cout << "Play locally against AI?    [" << playVsAi     << "]" << std::endl;

		std::string input;
		std::getline(std::cin, input);

		if (input.length() > 0)
		{
			const char in = input[0];

			if (in == playVsOnline)
			{
				Socket socket;
				bool listen = false;

				while (true)
				{
					std::cout << "Enter ip address or [n] to listen for someone." << std::endl;

					std::string online;
					std::getline(std::cin, online);

					for (int i = 0; i < online.length(); i++)
					{
						if (i == 0 && online[0] == 'n')
						{
							listen = true;
							break;
						}
					}

					if (listen)
						break;
				}

				if (listen)
				{
					socket.open(socketPort);
					std::cout << "Listening on port: " << socketPort << std::endl;

					// TODO: Add timeout for waiting for data to continue or exit
					while (true)
					{
						Address from;
						const char request = 'a';
						char data[1];
						
						if (socket.recieve(from, data, sizeof(data)) > 0 && data[0] == request)
						{
							std::cout << "Play with " << from.toString() << "? [y/n]" << std::endl;
							
							std::string response;
							std::getline(std::cin, response);

							if (response.length() > 0 && response[0] == 'y')
							{
								playOnline(game, from, socket, true);
								break;
							}
						}
					}
				}

				socket.close();
			}

			else if (in == playVsFriend)
				playLocally(game);

			else if (in == playVsAi)
			{
				std::cout << "Be player 1? [y/n]" << std::endl;

				std::string enableAi;
				std::getline(std::cin, enableAi);

				if (enableAi.length() > 0 && enableAi[0] == 'y')
					game.enableAi(false);

				else
					game.enableAi(true);

				playLocally(game);
			}

			else
				invalidInput = true;
		}

		else
			invalidInput = true;
	}

	Socket::shutdownSockets();
	std::cin.get();

	return 0;
}