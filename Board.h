#pragma once

#include <iostream>
#include <vector>

typedef std::vector<std::vector<uint8_t>> GameBoard;

struct Board {
	int width;
	int height;
	int minmaxVal;
	GameBoard board;
	std::vector<Board*> nextMoves;

	Board(int w, int h);

	Board(int w, int h, GameBoard gB);

	void move(Board& board, int move, uint8_t player);

	void undoMove(Board& board, int move, uint8_t player);

	bool isLegalMove(Board& board, int column);
};