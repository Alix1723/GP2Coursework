#ifndef _CBRICK_H
#define _CBRICK_H
#include "cSprite.h"

class cBrick : cSprite
{
private:
		int hits;
		cSprite brickSprite;
		D3DXVECTOR2 brickCentre;
		bool alive;
public:
	cBrick();
	cBrick(D3DXVECTOR3 sPosition,  LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR theFilename, int iHits);
	~cBrick();
	cSprite getBrickSprite();
	int getBrickHits();
	void setBrickHits(int iHits);
	float checkCollisionCoarse(D3DXVECTOR2 bPos);
	void hitBrick();
	D3DXVECTOR2 getBrickCentre();
	bool isAlive();
};

#endif