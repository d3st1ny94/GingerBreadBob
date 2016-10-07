
#ifndef _PIECES_H_
#define _PIECES_H_
class Pieces
{
public:
	Pieces();
	~Pieces();
	int GetBlockType(int, int, int, int);
	int GetXInitialPosition(int, int);
	int GetYInitialPosition(int, int);
};

#endif