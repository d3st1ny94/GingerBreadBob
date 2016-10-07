#ifndef _GAME_H_
#define _GAME_H_

#include "Board.h"
#include <time.h>
#define WAIT_TIME 700 
class Game
{
public:
	Game(Board *pBoard, Pieces *pPieces);
	void CreateNewPiece();
	
    int mPosX, mPosY;               // Position of the piece that is falling down
    int mPiece, mRotation;          // Kind and rotation the piece that is falling down
	std::vector<std::vector<int>> GetBoard(); //get blocks on board
	void HandleInput(int action, float deltatime);
	void Update(float deltatime);
	int GetScore();
	bool isPaused();
	bool isGameOver();
private:

	int mNextPosX, mNextPosY;       // Position of the next piece
	int mNextPiece, mNextRotation;  // Kind and rotation of the next piece
	bool mPaused = false;
	bool mGameOver = false;
	Board *mBoard;
	Pieces *mPieces;

	int GetRand(int pA, int pB);
	void InitGame();
	unsigned long mTime = 0;
	unsigned long mTimeSinceInput = 0;
};

#endif
