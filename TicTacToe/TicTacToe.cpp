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

void TicTacToe::enableAi(const bool enableAI, const bool asPlayerOne)
{
	m_aiEnabled = enableAI;
	m_aiPlayerOne = asPlayerOne;
}

const bool TicTacToe::aiEnabled() const
{
	return m_aiEnabled;
}

void TicTacToe::insertAiMove()
{
	int x;
	int y;
	int bestTurnVal = 0;

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
			int currTurnVal = getBestMove(m_board, (MAX_MOVES - m_movesMade) - 1);
			m_board[i][j] = m_emptySpot;

			std::cout << currTurnVal << " : " << (MAX_MOVES - m_movesMade) - 1 << " turnVal : " << m_k << " || " << m_i << " :: " << m_j << std::endl;

			if (firstVal || currTurnVal > bestTurnVal)
			{
				firstVal = false;
				bestTurnVal = currTurnVal;
				x = i;
				y = j;
			}
		}
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
	if (getWinner(m_board, m_playerOne))
		return m_playerOne;

	if (getWinner(m_board, m_playerTwo))
		return m_playerTwo;

	return m_emptySpot;
}

// TODO: Simplify this
const bool TicTacToe::getWinner(const char board[m_boardSize][m_boardSize], const char player) const
{
	// Horizontal
	for (int i = 0; i < m_boardSize; i++)
		if (board[i][0] == player && board[i][0] == board[i][1] && board[i][1] == board[i][2])
			return true;

	// Vertical
	for (int i = 0; i < m_boardSize; i++)
		if (board[0][i] == player && board[0][i] == board[1][i] && board[1][i] == board[2][i])
			return true;

	// Diagonal descending from left
	if (board[0][0] == player && board[0][0] == board[1][1] && board[1][1] == board[2][2])
		return true;

	// Diagonal ascending from left
	if (board[2][0] == player && board[2][0] == board[1][1] && board[1][1] == board[0][2])
		return true;

	return false;
}

const char TicTacToe::getSpot(const int x, const int y) const
{
	return m_board[x][y];
}

void TicTacToe::draw() const
{
	std::cout << std::endl;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			std::cout << "\t" << m_board[i][j] << "|\t";

		std::cout << std::endl;
	}

	std::cout << std::endl;
}

const int TicTacToe::getBestMove(char copyboard[m_boardSize][m_boardSize], const int turns)
{
	char board[m_boardSize][m_boardSize];

	for (int i = 0; i < m_boardSize; i++)
		for (int j = 0; j < m_boardSize; j++)
			board[i][j] = copyboard[i][j];

	int turnVals = 0;
	const bool playerOneWon = getWinner(board, m_playerOne);
	const bool playerTwoWon = getWinner(board, m_playerTwo);

	if (playerOneWon || playerTwoWon)
	{
		m_k++;
		
		if ((playerOneWon && m_aiPlayerOne) || (playerTwoWon &&  !m_aiPlayerOne))
		{
			m_i++;
			//return 1 * turns;
			turnVals = 1 * turns;
		}
		
		if ((playerOneWon && !m_aiPlayerOne) || (playerTwoWon &&  m_aiPlayerOne))
		{
			m_j++;
			//return -1 * turns;
			turnVals = -1 * turns;
		}
	}

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