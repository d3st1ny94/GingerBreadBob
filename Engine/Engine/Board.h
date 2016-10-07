#ifndef _BOARD_H_
#define _BOARD_H_

#include "Pieces.h"
#include <vector>
#define BOARD_WIDTH 7
#define BOARD_HEIGHT 8
#define PIECE_BLOCKS 5
class Board
{
public:
	Board(Pieces *pPieces);
	int GetScore();
	bool isFreeBlock(int pX, int pY);
	bool isPossibleMovement(int pX, int pY, int pPiece, int pRotation);
	bool isGameOver();
	void StorePiece(int pX, int pY, int pPiece, int pRotation);
	void Init();
	void DeletePossibleLines();
	std::vector<std::vector<int>>& GetBoard();
private:
	enum {POS_FREE, POS_FILLED };
	int mBoard[BOARD_WIDTH][BOARD_HEIGHT];
	Pieces *mPieces;
	void InitBoard();
	std::vector<std::vector<int>> mBoardVector;
	void DeleteLine(int pY);
	int mScore;
};

#endif