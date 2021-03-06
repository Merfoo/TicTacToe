#include <iostream>
#include <memory>

#include "TicTacToe.h"

TicTacToe::TicTacToe(const char playerOne, const char playerTwo, const char emptySpot) :
	m_playerOne(playerOne),
	m_playerTwo(playerTwo),
	m_emptySpot(emptySpot),
	m_aiEnabled(false),
	m_aiPlayerOne(false),
	m_movesMade(0)
{
	resetBoard();
}

TicTacToe::~TicTacToe()
{
}

void TicTacToe::resetBoard()
{
	for (int i = 0; i < m_boardSize; i++)
		for (int j = 0; j < m_boardSize; j++)
			m_board[i][j] = m_emptySpot;
}

void TicTacToe::enableAi(const bool aiPlayerOne)
{
	m_aiEnabled = true;
	m_aiPlayerOne = aiPlayerOne;
}

void TicTacToe::disableAi()
{
	m_aiEnabled = false;
}

const bool TicTacToe::aiEnabled() const
{
	return m_aiEnabled;
}

void TicTacToe::insertAiMove()
{
	int x;
	int y;

	if (m_movesMade > 0)
	{
		float bestTurnVal = 0;
		bool firstVal = true;

		for (int i = 0; i < m_boardSize; i++)
		{
			for (int j = 0; j < m_boardSize; j++)
			{
				if (m_board[i][j] != m_emptySpot)
					continue;

				m_i = 0;
				m_j = 0;
				m_k = 0;

				m_board[i][j] = m_aiPlayerOne ? m_playerOne : m_playerTwo;
				const int nextTurn = maxMoves - m_movesMade - 1;
				const float currTurnVal = getBestMove(m_board, nextTurn);
				m_board[i][j] = m_emptySpot;

				//std::cout << currTurnVal << " : " << nextTurn << " turnVal : " << m_k << " || " << m_i << " :: " << m_j << std::endl;

				if (firstVal || currTurnVal > bestTurnVal)
				{
					firstVal = false;
					bestTurnVal = currTurnVal;
					x = i;
					y = j;
				}
			}
		}
	}

	else
	{
		x = 1;
		y = 1;
	}

	insertMove(x, y);
}

const bool TicTacToe::insertMove(const int x, const int y)
{
	if (m_board[x][y] != m_emptySpot)
		return false;

	m_board[x][y] = isPlayerOneTurn() ? m_playerOne : m_playerTwo;
	m_movesMade++;

	return true;
}

const bool TicTacToe::isAiTurn() const
{
	return m_aiEnabled ? m_aiPlayerOne == isPlayerOneTurn() : false;
}

const bool TicTacToe::isPlayerOneTurn() const
{
	return m_movesMade % 2 == 0;
}

const char TicTacToe::getEmptySpot() const
{
	return m_emptySpot;
}

const char TicTacToe::getWinner() const
{
	return getWinner(m_board);
}

// TODO: Simplify this
const char TicTacToe::getWinner(const char board[m_boardSize][m_boardSize]) const
{
	// Horizontal
	for (int i = 0; i < m_boardSize; i++)
		if (board[i][0] != m_emptySpot && board[i][0] == board[i][1] && board[i][1] == board[i][2])
			return board[i][0];

	// Vertical
	for (int i = 0; i < m_boardSize; i++)
		if (board[0][i] != m_emptySpot && board[0][i] == board[1][i] && board[1][i] == board[2][i])
			return board[0][i];

	// Diagonal descending from left
	if (board[0][0] != m_emptySpot && board[0][0] == board[1][1] && board[1][1] == board[2][2])
		return board[0][0];

	// Diagonal ascending from left
	if (board[2][0] != m_emptySpot && board[2][0] == board[1][1] && board[1][1] == board[0][2])
		return board[2][0];

	return m_emptySpot;
}

const char TicTacToe::getSpot(const int x, const int y) const
{
	return m_board[x][y];
}

const char TicTacToe::getPlayerOne() const
{
	return m_playerOne;
}

const char TicTacToe::getPlayerTwo() const
{
	return m_playerTwo;
}

const int TicTacToe::getMovesMade() const
{
	return m_movesMade;
}

void TicTacToe::draw() const
{
	/*
		X | O | O
	   -----------
	    X | O | X
	   -----------
	    O | X | X
	*/

	std::cout << std::endl;

	for (int i = 0; i < 3; i++)
	{
		std::cout << "\t\n  ";

		for (int j = 0; j < 3; j++)
		{
			std::cout << m_board[i][j];

			if (j < 2)
				std::cout << " | ";
		}

		std::cout << "\n ";
		
		if(i < 2)
			for (int j = 0; j < 11; j++)
				std::cout << "_";

		std::cout << std::endl;
	}

	std::cout << std::endl;
}

const float TicTacToe::getBestMove(const char copyboard[m_boardSize][m_boardSize], const int turns)
{
	const char winner = getWinner(copyboard);

	if (winner != m_emptySpot || turns <= 0)
	{
		m_k++;
		
		// AI wins
		if ((winner == m_playerOne && m_aiPlayerOne) || (winner == m_playerTwo &&  !m_aiPlayerOne))
		{
			m_i++;

			if (maxMoves - (turns + 1) == m_movesMade)
			{
				//std::cout << "MMAANNNNN" << std::endl;
				// so bad, i cri evryteim :3
				return turns + 1000000;
			}

			return 0.5 * (turns + 1);
		}
		
		// Non-AI wins
		if ((winner == m_playerOne && !m_aiPlayerOne) || (winner == m_playerTwo &&  m_aiPlayerOne))
		{
			m_j++;

			// If previous turn was Non-AI and they win, put more value in this spot to prevent them winning
			if (((turns + 1) % 2 != 0) == m_aiPlayerOne)
				return 0.5 * (turns + 1);
			
			return -1 * (turns + 1);
		}

		return 0;
	}

	char board[m_boardSize][m_boardSize];

	for (int i = 0; i < m_boardSize; i++)
		for (int j = 0; j < m_boardSize; j++)
			board[i][j] = copyboard[i][j];

	float turnVals = 0;

	for (int i = 0; i < m_boardSize; i++)
	{
		for (int j = 0; j < m_boardSize; j++)
		{
			if (board[i][j] != m_emptySpot)
				continue;

			board[i][j] = turns % 2 != 0 ? m_playerOne : m_playerTwo;
			turnVals += getBestMove(board, turns - 1);
			board[i][j] = m_emptySpot;
		}
	}

	return turnVals;
}