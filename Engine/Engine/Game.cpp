#include "Game.h"

Game::Game(Board *pBoard, Pieces *pPieces)
{

	// Get the pointer to the Board and Pieces classes
	mBoard = pBoard;
	mPieces = pPieces;

	// Game initialization
	InitGame();
}

int Game::GetRand(int pA, int pB)
{
	return rand() % (pB - pA + 1) + pA; //get random piece
}

void Game::InitGame()
{
	// Init random numbers
	srand((unsigned int)time(NULL));

	//Init Board;
	mBoard->Init();

	// First piece
	mPiece = GetRand(0, 6);
	mRotation = GetRand(0, 3);
	mPosX = (BOARD_WIDTH / 2) + mPieces->GetXInitialPosition(mPiece, mRotation);
	mPosY = mPieces->GetYInitialPosition(mPiece, mRotation);

	//  Next piece
	mNextPiece = GetRand(0, 6);
	mNextRotation = GetRand(0, 3);
	mNextPosX = BOARD_WIDTH + 5;
	mNextPosY = 5;
}

void Game::CreateNewPiece()
{
	// The new piece
	mPiece = mNextPiece;
	mRotation = mNextRotation;
	mPosX = (BOARD_WIDTH / 2) + mPieces->GetXInitialPosition(mPiece, mRotation);
	mPosY = mPieces->GetYInitialPosition(mPiece, mRotation);

	// Random next piece
	mNextPiece = GetRand(0, 6);
	mNextRotation = GetRand(0, 3);
}

std::vector<std::vector<int>> Game::GetBoard(){
	std::vector<std::vector<int>> ret = mBoard->GetBoard();
	 //Store each block of the current piece into the board
	for (int i1 = mPosX, i2 = 0; i1 < mPosX + PIECE_BLOCKS; i1++, i2++)
	{
		for (int j1 = mPosY, j2 = 0; j1 < mPosY + PIECE_BLOCKS; j1++, j2++)
		{
			// Store only the blocks of the current piece that are not holes
			if (mPieces->GetBlockType(mPiece, mRotation, j2, i2) != 0)
			{
				if (j1 >= 0 && i1 >= 0)
				{
					ret.at(j1).at(i1) = 1;
				}
			}
		}
	}
	return ret;
}
void Game::HandleInput(int action, float deltaTime){
	mTimeSinceInput -= deltaTime * 1000;
	if (mTimeSinceInput <= 0)
	{
		mTimeSinceInput = 100;
		switch (action){
			case 0: //right
			{
				if (mBoard->isPossibleMovement(mPosX + 1, mPosY, mPiece, mRotation))
					mPosX++;
				break;
			}

			case 1: // left
			{
				if (mBoard->isPossibleMovement(mPosX - 1, mPosY, mPiece, mRotation))
					mPosX--;
				break;
			}

			case 2: // up (rotate)
			{
				if (mBoard->isPossibleMovement(mPosX, mPosY, mPiece, (mRotation + 1) % 4))
					mRotation = (mRotation + 1) % 4;

				break;
			}
			case 3: // down (instant put)
			{
				// Check collision from up to down
				while (mBoard->isPossibleMovement(mPosX, mPosY, mPiece, mRotation)) { mPosY++; }

				mBoard->StorePiece(mPosX, mPosY - 1, mPiece, mRotation);
				//check for lines
				mBoard->DeletePossibleLines();

				if (mBoard->isGameOver())
				{
					//won tally up the game
					InitGame();
				}

				CreateNewPiece();

				break;
			}
		}
	}
}
void Game::Update(float deltatime){
	mTime += deltatime*1000;
	if (mTime > WAIT_TIME){
		mTime = 0; 
		if (mBoard->isPossibleMovement(mPosX, mPosY + 1, mPiece, mRotation))
		{
			//we can move down, continue with this piece
			mPosY++;
		}
		else
		{
			//store current piece in board
			mBoard->StorePiece(mPosX, mPosY, mPiece, mRotation);
			//check for lines
			mBoard->DeletePossibleLines();

			if (mBoard->isGameOver())
			{
				//won tally up the game
				InitGame();
			}

			CreateNewPiece();
		}

	}
}
int Game::GetScore(){
	return mBoard->GetScore();
}