#include "Board.h"

Board::Board(int w, int h) {
	width = w;
	height = h;
	minmaxVal = 0;

	board.resize(width);
	for (int i = 0; i < width; i++) {
		board.at(i).resize(height);
		nextMoves.push_back(NULL);
	}

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			board.at(i).at(j) = 0;
		}
	}
}

Board::Board(int w, int h, GameBoard gB) {
	width = w;
	height = h;
	minmaxVal = 0;

	board.resize(width);
	for (int i = 0; i < width; i++) {
		board.at(i).resize(height);
		nextMoves.push_back(NULL);
	}

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			board.at(i).at(j) = gB.at(i).at(j);
		}
	}
}

void Board::move(Board& board, int move, uint8_t player) {
	//Board newBoard(board.width, board.height, board.board);

	for (int i = height; i > 0; i--) {
		if (board.board.at(move).at(i-1) == 0) {
			board.board.at(move).at(i-1) = player;
			return;
		}
	}

	//return newBoard;
}

void Board::undoMove(Board& board, int move, uint8_t player) {
	//Board newBoard(board.width, board.height, board.board);

	for (int i = 0; i < board.width; i++) {
		if (board.board.at(move).at(i) != 0) {
			board.board.at(move).at(i) = 0;
			return;
		}
	}

	//return newBoard;
}

bool Board::isLegalMove(Board& board, int column) {
	if (column < 0 || column > board.width || board.board.at(column).at(0) != 0 /*or replace what ever is the empty space*/) {
		std::cout << "Column " << column << " is not empty" << std::endl;
		return false;
	}
	std::cout << "Spot at column " << column << " is empty" << std::endl;
	return true;
}