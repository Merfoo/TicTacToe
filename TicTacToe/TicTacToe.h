#pragma once

class TicTacToe
{
public:
	TicTacToe(const char playerOne, const char playerTwo, const char emptySpot);
	~TicTacToe();
	
	void resetBoard();
	
	void enableAi(const bool enableAI, const bool asPlayerOne);
	const bool aiEnabled() const;

	void insertAiMove();
	const bool insertMove(const int x, const int y);

	const bool isAiTurn() const;
	const bool isPlayerOneTurn() const;

	const char getEmptySpot() const;
	const char getWinner() const;
	const char getSpot(const int x, const int y) const;
private:
	char m_playerOne;
	char m_playerTwo;
	char m_emptySpot;

	static const int m_boardSize = 3;
	char m_board[m_boardSize][m_boardSize];

	bool m_aiEnabled;
	bool m_aiPlayerOne;
	bool m_playerOneTurn;

	static const int m_maxMoves = 9;
	int m_movesMade;

	const int getBestMove(char copyboard[m_boardSize][m_boardSize], const int turns);
	const char getWinner(const char board[m_boardSize][m_boardSize]) const;
	int m_i;
	int m_j;
	int m_k;
};
