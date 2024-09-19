#include "Board.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <new>
#include <chrono>
#include <vector>
#include <string>
using namespace std::chrono;

void printBoard(Board board);

void game(Board& board, bool playermove, bool AImove);

//void minmax(Board board);

bool checkVictory(Board& board);

void bestMove(Board& board);

int minimax(Board& board, bool minimizing);

//int newMinimax(Board& board, int depth, bool max, int alpha, int beta);

//int utility(Board& board);

void generate(Board& board, int _level);

int aiMoveNumber = 0;
int opponentMoveNumber = 0;
typedef int* IntPtrType;
//int numberOfNodesCreated = 0;
//int numberOfNodesSearched = 0;
//auto start = high_resolution_clock::now();
//auto stop = high_resolution_clock::now();
//std::vector<int> moveTime;

int main(void) {
	int width = 0;
	int height = 0;
	//std::cout << "Enter board width: ";
	//std::cin >> width;
	//std::cout << std::endl;
	//std::cout << "Enter board height: ";
	//std::cin >> height;
	//std::cout << std::endl;

	//Board board(width, height);
	//std::cout << board.board.at(0).at(0);
	//game(board, true, false);

    //std::string fileName = "output";

    //srand((unsigned)time(0));
    //int randomNumber = std::rand();
    //std::string randNumString = std::to_string(randomNumber);
    //fileName += randNumString;

    //fileName += ".txt";

    //std::ofstream outputFile;
    //outputFile.open(fileName);
    //if (outputFile.fail()) {
    //    std::cout << "Failed to open output file.\n";
    //    exit(0);
    //}

    char yesNo;
    while (true) {
        std::cout << "Welcome to AI Connect Four!!\n\n";

        std::cout << "Enter the board width: ";
        std::cin >> width;
        std::cout << std::endl;
        std::cout << "Enter board height: ";
        std::cin >> height;
        std::cout << std::endl;

        //outputFile << height << "x" << width << std::endl;

        std::cout << "Are you going first? (Y/N): ";
        std::cin >> yesNo;
        yesNo = toupper(yesNo);
        if (yesNo == 'Y') {
            Board board(width, height);
            std::cout << "Your first move cannot be in the corners or the middle.\n";
            game(board, true, false);
        }
        else {
            Board board(width, height);
            game(board, false, true);
        }

        //outputFile << "Number of nodes created: " << numberOfNodesCreated << std::endl;
        //outputFile << "Number of nodes searched: " << numberOfNodesSearched << std::endl;
        //for(int i = 0; i < aiMoveNumber; i++)
            //outputFile << "Duration of move #" << i + 1 << ": " << moveTime.at(i) << std::endl;

        std::cout << "Would you like to play again? (Y/N): ";
        std::cin >> yesNo;
        yesNo = toupper(yesNo);
        if (yesNo == 'N') {
            std::cout << "Ending...\n";

            //outputFile.close();
            //if (outputFile.fail()) {
            //    std::cout << "Failed to close output file.\n";
            //    exit(0);
            //}

            break;
        }

        aiMoveNumber = 0;
        opponentMoveNumber = 0;
        //numberOfNodesCreated = 0;
        //numberOfNodesSearched = 0;
    }

	return 0;
}

void printBoard(Board board) {
	for (int i = 0; i < board.height; i++) {
		for (int j = 0; j < board.width; j++) {
			std::cout << (int) board.board.at(j).at(i);
			if (j != (board.width - 1)) {
				std::cout << "|";
			}
		}
		std::cout << std::endl;
		if (i != (board.height - 1)) {
			for (int k = 0; k < board.width; k++) {
				std::cout << "--";
			}
			std::cout << std::endl;
		}
	}
}

void game(Board& board, bool playermove, bool AImove) {
	bool victory = false;
	int moveLoc;

	while (!victory) {
        bool tie = true;
        for (int i = 0; i < board.width; i++) {
            if (board.board.at(i).at(0) == 0) {
                tie = false;
                break;
            }
        }
        if (tie) {
            printBoard(board);
            std::cout << "Tie!";
            std::cout << std::endl;
            return;
        }
        
        printBoard(board);
		if (playermove) {
			std::cout << "Choose column to play in (1-" << board.width << "): ";
			std::cin >> moveLoc;
			std::cout << std::endl;
			board.move(board, moveLoc - 1, 1);
            opponentMoveNumber++;
		}
		if (AImove) {
			std::cout << "AI is moving...";
			std::cout << std::endl;
            bestMove(board);
            //stop = high_resolution_clock::now();
            //auto calculated_duration = duration_cast<seconds>(stop - start);
            //moveTime.push_back(calculated_duration.count());
            aiMoveNumber++;
		}

	    victory = checkVictory(board);

		if (victory) {
            if (playermove) {
                printBoard(board);
                std::cout << "Player wins!";
                std::cout << std::endl;
            }
			else if (AImove) {
                printBoard(board);
				std::cout << "AI wins!";
                std::cout << std::endl;
			}
		}
		else {
			playermove = !playermove;
			AImove = !AImove;
		}
	}
}

//bool checkVictory(Board board) {
//	uint8_t space = 0;
//	for (int i = 0; i < board.width; i++) {
//		for (int j = board.height - 1; j >= 0; j--) {
//			if (board.board.at(i).at(j) != 0) {
//				space = board.board.at(i).at(j);
//				uint8_t count = 1;
//			}
//		}
//	}
//	return false;
//}

// helper function to check for a horizontal win
int checkHorizontalWin(Board& board, int col, int row) {
    // get the color of the current piece
    int color = board.board.at(col).at(row);
    // initialize a counter
    int count = 1;
    // check the left side of the current piece
    for (int i = col - 1; i >= 0; i--) {
        // if the color is the same, increment the counter
        if (board.board.at(i).at(row) == color) {
            count++;
        }
        // otherwise, break the loop
        else {
            break;
        }
    }
    // check the right side of the current piece
    for (int i = col + 1; i < board.width; i++) {
        // if the color is the same, increment the counter
        if (board.board.at(i).at(row) == color) {
            count++;
        }
        // otherwise, break the loop
        else {
            break;
        }
    }
    // if the counter is 4, there is a horizontal win
    if (count == 4) {
        // return the color of the winner
        if (color == 2)
            board.minmaxVal += 100;
        else if (color == 1)
            board.minmaxVal += -100;
        return color;
    }
    // otherwise, return 0
    else if (count == 3) {
        if (color == 2)
            board.minmaxVal += 9;
        else if (color == 1)
            board.minmaxVal += -9;
    }
    else if (count == 2) {
        if (color == 2)
            board.minmaxVal += 2;
        else if (color == 1)
            board.minmaxVal += -2;
    }
    else if (count == 1) {
        if (color == 2)
            board.minmaxVal += 1;
        else if (color == 1)
            board.minmaxVal += -1;
    }
    return 0;
}

// helper function to check for a vertical win
int checkVerticalWin(Board& board, int col, int row) {
    // get the color of the current piece
    int color = board.board.at(col).at(row);
    // initialize a counter
    int count = 1;
    // check the bottom side of the current piece
    for (int i = row + 1; i < board.height; i++) {
        // if the color is the same, increment the counter
        if (board.board.at(col).at(i) == color) {
            count++;
        }
        // otherwise, break the loop
        else {
            break;
        }
    }
    // check the top side of the current piece
    for (int i = row - 1; i >= 0; i--) {
        // if the color is the same, increment the counter
        if (board.board.at(col).at(i) == color) {
            count++;
        }
        // otherwise, break the loop
        else {
            break;
        }
    }
    // if the counter is 4, there is a vertical win
    if (count == 4) {
        // return the color of the winner
        if (color == 2)
            board.minmaxVal += 100;
        else if (color == 1)
            board.minmaxVal += -100;
        return color;
    }
    // otherwise, return 0
    else if (count == 3) {
        if (color == 2)
            board.minmaxVal += 9;
        else if (color == 1)
            board.minmaxVal += -9;
    }
    else if (count == 2) {
        if (color == 2)
            board.minmaxVal += 2;
        else if (color == 1)
            board.minmaxVal += -2;
    }
    else if (count == 1) {
        if (color == 2)
            board.minmaxVal += 1;
        else if (color == 1)
            board.minmaxVal += -1;
    }
    return 0;
}

// helper function to check for an ascending diagonal win
int checkAscendingDiagonalWin(Board& board, int col, int row) {
    // get the color of the current piece
    int color = board.board.at(col).at(row);
    // initialize a counter
    int count = 1;
    // check the bottom left side of the current piece
    for (int i = col - 1, j = row + 1; i >=0 && j < board.height; i--, j++) {
        // if the color is the same, increment the counter
        if (board.board.at(i).at(j) == color) {
            count++;
        }
        // otherwise, break the loop
        else {
            break;
        }
    }
    // check the top right side of the current piece
    for (int i = col + 1, j = row - 1; i < board.width && j >= 0; i++, j--) {
        // if the color is the same, increment the counter
        if (board.board.at(i).at(j) == color) {
            count++;
        }
        // otherwise, break the loop
        else {
            break;
        }
    }
    // if the counter is 4, there is an ascending diagonal win
    if (count == 4) {
        // return the color of the winner
        if (color == 2)
            board.minmaxVal += 100;
        else if (color == 1)
            board.minmaxVal += -100;
        return color;
    }
    // otherwise, return 0
    else if (count == 3) {
        if (color == 2)
            board.minmaxVal += 9;
        else if (color == 1)
            board.minmaxVal += -9;
    }
    else if (count == 2) {
        if (color == 2)
            board.minmaxVal += 2;
        else if (color == 1)
            board.minmaxVal += -2;
    }
    else if (count == 1) {
        if (color == 2)
            board.minmaxVal += 1;
        else if (color == 1)
            board.minmaxVal += -1;
    }
    return 0;
}

// helper function to check for a descending diagonal win
int checkDescendingDiagonalWin(Board& board, int col, int row) {
    // get the color of the current piece
    int color = board.board.at(col).at(row);
    // initialize a counter
    int count = 1;
    // check the bottom right side of the current piece
    for (int i = col + 1, j = row + 1; i < board.width && j < board.height; i++, j++) {
        // if the color is the same, increment the counter
        if (board.board.at(i).at(j) == color) {
            count++;
        }
        // otherwise, break the loop
        else {
            break;
        }
    }
    // check the top left side of the current piece
    for (int i = col - 1, j = row - 1; i >= 0 && j >= 0; i--, j--) {
        // if the color is the same, increment the counter
        if (board.board.at(i).at(j) == color) {
            count++;
        }
        // otherwise, break the loop
        else {
            break;
        }
    }
    // if the counter is 4, there is a descending diagonal win
    if (count == 4) {
        // return the color of the winner
        if (color == 2)
            board.minmaxVal += 100;
        else if (color == 1)
            board.minmaxVal += -100;
        return color;
    }
    // otherwise, return 0
    else if (count == 3) {
        if (color == 2)
            board.minmaxVal += 9;
        else if (color == 1)
            board.minmaxVal += -9;
    }
    else if (count == 2) {
        if (color == 2)
            board.minmaxVal += 2;
        else if (color == 1)
            board.minmaxVal += -2;
    }
    else if (count == 1) {
        if (color == 2)
            board.minmaxVal += 1;
        else if (color == 1)
            board.minmaxVal += -1;
    }
    return 0;
}

// main function to check for a victory in Connect 4
bool checkVictory(Board& board) {
    // iterate over the rows
    for (int col = 0; col < board.width; col++) {
        // iterate over the columns
        for (int row = 0; row < board.height; row++) {
            // get the color of the current piece
            int color = board.board.at(col).at(row);
            // if the color is not 0 (empty)
            if (color != 0) {
                // check for a horizontal win
                int horizontalWin = checkHorizontalWin(board, col, row);
                // if there is a horizontal win, return the color of the winner
                if (horizontalWin != 0) {
                    return true;
                }
                // check for a vertical win
                int verticalWin = checkVerticalWin(board, col, row);
                // if there is a vertical win, return the color of the winner
                if (verticalWin != 0) {
                    return true;
                }
                // check for an ascending diagonal win
                int ascendingDiagonalWin = checkAscendingDiagonalWin(board, col, row);
                // if there is an ascending diagonal win, return the color of the winner
                if (ascendingDiagonalWin != 0) {
                    return true;
                }
                // check for a descending diagonal win
                int descendingDiagonalWin = checkDescendingDiagonalWin(board, col, row);
                // if there is a descending diagonal win, return the color of the winner
                if (descendingDiagonalWin != 0) {
                    return true;
                }
            }
        }
    }
    // if there is no win, return 0
    return false;
}

void bestMove(Board& board) {
    //start = high_resolution_clock::now();

    // make random move if going first and is first move
    // make random move if going second and is first move
    if ((aiMoveNumber == 0 && opponentMoveNumber == 0) || (aiMoveNumber == 0 && opponentMoveNumber == 1)) {
        int move = std::rand() % board.width;
        int middle = board.width / 2;
        int middle2 = 0;

        // eliminate middle if even number
        if (board.width % 2 == 0) {
            middle2 = middle - 1;
        }

        // make move
        if (move != 0 && move != board.width - 1 && move != middle && move != middle2) {
            board.move(board, move, 2);
            std::cout << "Move in column " << move + 1 << std::endl;
            return;
        }
        else {
            board.move(board, 1, 2);
            std::cout << "Move in column " << 2 << std::endl;
            return;
        }
    }
    else if (board.width > 9) {
        int move = std::rand() % board.width;
        board.move(board, move, 2);
        std::cout << "Move in column " << move << std::endl;
        return;
    }
    else {
        for (int k = 0; k < board.width; k++) {
            Board newBoard = Board(board.width, board.height, board.board);
            newBoard.move(newBoard, k, 2);
            if (checkVictory(newBoard)) {
                board.move(board, k, 2);
                std::cout << "Move in column " << k + 1 << std::endl;
                return;
            }
        }
        for (int n = 0; n < board.width; n++) {
            Board newBoard = Board(board.width, board.height, board.board);
            newBoard.move(newBoard, n, 1);
            if (checkVictory(newBoard)) {
                board.move(board, n, 2);
                std::cout << "Move in column " << n + 1 << std::endl;
                return;
            }
        }
        for (int j = 2; j < board.width - 2; j++) {
            if (board.board.at(j).at(board.height - 1) == 1 && board.board.at(j + 1).at(board.height - 1) == 1 || board.board.at(j).at(board.height - 1) == 1 && board.board.at(j - 1).at(board.height - 1) == 1) {
                if (board.board.at(j + 2).at(board.height - 1) == 0) {
                    board.move(board, j + 2, 2);
                    std::cout << "Move in column " << j + 2 << std::endl;
                }
                else if (board.board.at(j - 2).at(board.height - 1) == 0) {
                    board.move(board, j - 2, 2);
                    std::cout << "Move in column " << j - 2 << std::endl;
                }
            }
        }
        generate(board, 1);
        int value = minimax(board, false);
        for (int i = 0; i < board.width; i++) {
            if (board.nextMoves.at(i) != NULL) {
                if (board.nextMoves.at(i)->minmaxVal == value) {
                    if (board.board.at(i).at(0) == 0) {
                        board.move(board, i, 2);
                        std::cout << "Move in column " << i + 1 << std::endl;
                        break;
                    }
                    else {
                        for (int n = 0; n < board.width; n++) {
                            if (board.board.at(n).at(0) == 0) {
                                board.move(board, n, 2);
                                std::cout << "Move in column " << n + 1 << std::endl;
                                break;
                            }
                        }
                    }
                    break;
                }
            }
        }
        return;
    }

}

void generate(Board& board, int _level) {
    //int level = _level;
    if (_level > 7) {
        return;
    }
    for (int i = 0; i < board.width; i++) {
        if (board.board.at(i).at(0) == 0) {
            board.nextMoves.at(i) = new Board(board.width, board.height, board.board);
            //numberOfNodesCreated++;
            if (_level % 2 == 1) {
                board.nextMoves.at(i)->move(*board.nextMoves.at(i), i, 2);
            }
            else {
                board.nextMoves.at(i)->move(*board.nextMoves.at(i), i, 1);
            }

            if (checkVictory(*board.nextMoves.at(i))) {
                return;
            }
            generate(*board.nextMoves.at(i), _level + 1);
        }
        else {
            board.nextMoves.at(i) = NULL;
            return;
        }
    }
}

int minimax(Board& board, bool minimizing) {
    //minimaxStart = high_resolution_clock::now();
    //generate(board, 1);
    //minimaxStart = high_resolution_clock::now();

    std::vector<int> values;
    values.resize(board.width);

    for (int i = 0; i < board.width; i++) {
        int count = 0;
        Board* boardPtr = board.nextMoves.at(i);
        if (boardPtr == NULL) {
            count++;
            if (count == board.nextMoves.size()) {
                return board.minmaxVal;
            }
        }
        else {
            values.at(i) = minimax(*boardPtr, !minimizing);
        }
    }
    int minmax = values.at(0);
    if (!minimizing) {
        for (int i = 0; i < board.width; i++) {
            if (values.at(i) > minmax) {
                minmax = values.at(i);
                //numberOfNodesSearched++;
            }
        }
    }
    else {
        for (int i = 0; i < board.width; i++) {
            if (values.at(i) < minmax) {
                minmax = values.at(i);
                //numberOfNodesSearched++;
            }
        }
    }
    board.minmaxVal = minmax;
    return board.minmaxVal;
}

//int newMinimax(Board& board, int depth, bool max, int alpha, int beta) {
//    int bestValue = 0;
//    int bestMove = 0;
//    int* bestMoves = new int[2];
//    //int *actions = new int[board.width];
//
//    IntPtrType actions;
//    try
//    {
//        actions = new int[board.width];
//    }
//    catch (std::bad_alloc)
//    {
//        std::cout << "Could not allocate sufficient space" << std::endl;
//        exit(1);
//    }
//
//    if (max) {
//        bestValue = -10000;
//        if (checkVictory(board) || depth == 0) {
//            std::cout << "The depth is 0 " << std::endl;
//            return utility(board);
//        }
//        
//        // actions = legalActions() -- go through the tree
//        for (int i = 0; i < board.width; i++) {
//            if (board.isLegalMove(board, i))
//                actions[i] = i;
//        }
//        
//        // for each action in actions
//        for (int i = 0; i < board.width; i++) {
//            // Make move at column action
//            board.move(board, i, 2);
//            // Calculate value by calling minimax (below)
//            int value = newMinimax(board, depth - 1, !max, alpha, beta);
//            // if value > bestValue
//            if (value > bestValue) {
//                // bestValue = value
//                bestValue = value;
//                // bestMove = newMove
//                bestMove = i;
//            }
//            
//            //undo move in the board
//            board.undoMove(board, i, 2);
//            delete[] actions;
//            bestMoves[0] = bestMove;
//            bestMoves[1] = bestValue;
//            alpha = newMinimax(board, depth - 1, max, alpha, beta);
//            if (bestValue >= beta)
//                return  *bestMoves;
//        }
//        // return bestMove, bestValue
//        return *bestMoves;
//        //int value = minimax(board, depth - 1, !max);
//    }
//
//    //min
//    else {
//        bestValue = 10000;
//        if (checkVictory(board) || depth == 0) {
//            return utility(board);
//        }
//        // actions = legalActions() -- go through the tree
//        for (int i = 0; i < board.width; i++) {
//            if (board.isLegalMove(board, i))
//                actions[i] = i;
//        }
//        // for each action in actions
//        for (int i = 0; i < board.width; i++) {
//            // Make move at column action
//            board.move(board, i, 2);
//            // Calculate value by calling minimax (below)
//            int value = newMinimax(board, depth - 1, max, alpha, beta);
//            // if value > bestValue
//            if (value > bestValue) {
//                // bestValue = value
//                bestValue = value;
//                // bestMove = newMove
//                bestMove = i;
//            }
//            //undo move in the board
//            board.undoMove(board, i, 2);
//            delete[] actions;
//            bestMoves[0] = bestMove;
//            bestMoves[1] = bestValue;
//            beta = newMinimax(board, depth - 1, !max, alpha, beta);
//            if (bestValue <= alpha)
//                return  *bestMoves;
//
//        }
//        // return bestMove, bestValue
//        return *bestMoves;
//    }
//}
//
////v stands for the value of the heuristic at a node
//int utility(Board& board) {
//    //heuristic
//    std::cout << "Went through utilitiy" << std::endl;
//    return 0;
//}