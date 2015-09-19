#include <iostream>
#include <string>
#include <chrono>

#include "TicTacToe.h"
#include "Address.h"
#include "Socket.h"

const int socketPort = 5000;
const float waitForPlayerTimeout = 10;		// Seconds
const float listenForPlayerTimeout = 20;	// Seconds

const float convertToSeconds(std::chrono::duration<float> duration)
{
	return duration.count();
}

// TODO: Add returning IDs for input like "exit" or "reset" instead of just 0
const int getPlayerCoords(const TicTacToe& game, int& x, int& y)
{
	bool invalidInput = false;

	while (true)
	{
		game.draw();

		if (invalidInput)
			std::cout << "Previous input invalid!\n";
		
		std::cout << "Your turn (" << (game.isPlayerOneTurn() ? game.getPlayerOne() : game.getPlayerTwo()) << ") \n";

		invalidInput = true;
		std::string input;
		std::getline(std::cin, input);

		if (input.length() >= 2)
		{
			x = (int(input[1]) - 48) - 1;
			y = (int(input[0]) - 48) - 1;

			if (x >= 0 && x < 3 && y >= 0 && y < 3)
				if (game.getSpot(x, y) == game.getEmptySpot())
					invalidInput = false;
		}

		if (!invalidInput)
			break;
	}

	return 0;
}

const bool checkGameStatus(const TicTacToe& game)
{
	const char winner = game.getWinner();

	if (winner != game.getEmptySpot())
	{
		game.draw();
		std::cout << " WINNER! " << winner << std::endl;
		
		return true;
	}

	else if (game.getMovesMade() == TicTacToe::maxMoves)
	{
		game.draw();
		std::cout << "\n CATS GAME! \n" << std::endl;

		return true;
	}

	return false;
}

void playOnline(TicTacToe game, const Address dest, Socket socket, const bool asPlayerOne)
{
	while (!checkGameStatus(game))
	{
		int x = 0;
		int y = 0;

		if (game.isPlayerOneTurn() == asPlayerOne)
		{
			getPlayerCoords(game, x, y);
			const char data[] = {x, y};

			socket.send(dest, data, sizeof(data));
		}

		else
		{
			auto startTime = std::chrono::steady_clock::now();

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

				if (convertToSeconds(std::chrono::steady_clock::now() - startTime) >= waitForPlayerTimeout)
				{
					std::cout << "Its been some time for the other player to pick a move, keep waiting? [y]" << std::endl;

					std::string continuePlaying;
					std::getline(std::cin, continuePlaying);

					if (continuePlaying.length() > 0 && continuePlaying[0] == 'y')
					{
						startTime = std::chrono::steady_clock::now();
						std::cout << "Continuing waiting..." << std::endl;
					}

					else
						return;
				}
			}
		}

		game.insertMove(x, y);
	}
}

void playLocally(TicTacToe game)
{
	while (!checkGameStatus(game))
	{
		if ((game.aiEnabled() && !game.isAiTurn()) || !game.aiEnabled())
		{
			int x, y;

			getPlayerCoords(game, x, y);
			game.insertMove(x, y);
		}

		else
			game.insertAiMove();
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

					auto startTime = std::chrono::steady_clock::now();

					// TODO: Add timeout for waiting for data to continue or exit
					while (true)
					{
						Address from;
						const char request = 'a';
						char data[1];
						
						if (socket.recieve(from, data, sizeof(data)) > 0 && data[0] == request)
						{
							std::cout << "Play with " << from.toString() << "? [y]" << std::endl;
							
							std::string response;
							std::getline(std::cin, response);

							if (response.length() > 0 && response[0] == 'y')
							{
								playOnline(game, from, socket, true);
								break;
							}
						}

						if (convertToSeconds(std::chrono::steady_clock::now() - startTime) >= listenForPlayerTimeout)
						{
							std::cout << "Continue listening? [y]" << std::endl;

							std::string continueListening;
							std::getline(std::cin, continueListening);

							if (continueListening.length() > 0 && continueListening[0] == 'y')
							{
								startTime = std::chrono::steady_clock::now();
								std::cout << "Continuing listening..." << std::endl;
							}

							else
								break;
						}
					}
				}

				socket.close();
			}

			else if (in == playVsFriend)
				playLocally(game);

			else if (in == playVsAi)
			{
				std::cout << "Be player 1? [y]" << std::endl;

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