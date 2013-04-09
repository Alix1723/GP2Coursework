#include "cBrick.h"
#include "cD3DManager.h"
#include <sstream>
/*
Definition for the cBrick class.
*/

cBrick::cBrick()
{
	cBrick::brickSprite = cSprite();
	cBrick::hits = 1;
	cBrick::brickSprite.setSpriteCentre();
	cBrick::brickCentre = cBrick::brickSprite.getSpriteCentre();
	cBrick::alive = true;
}

cBrick::cBrick(D3DXVECTOR3 sPosition,  LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR theFilename, int iHits)
{
	cBrick::brickSprite = cSprite(sPosition,pd3dDevice,theFilename);
	cBrick::hits = iHits;
	cBrick::brickSprite.setSpriteCentre();
	cBrick::brickCentre = (D3DXVECTOR2)sPosition + D3DXVECTOR2(16,2);
	cBrick::alive = true;
}

cSprite cBrick::getBrickSprite()
{
	return cBrick::brickSprite;
}

int cBrick::getBrickHits()
{
	return cBrick::hits;
}

void cBrick::setBrickHits(int iHits)
{
	if(iHits>0)
	{
		hits = iHits;
	}
	else
	{
		alive=false;
	}
}

D3DXVECTOR2 cBrick::getBrickCentre()
{
return cBrick::brickCentre;
}

float cBrick::checkCollisionCoarse(D3DXVECTOR2 bPos)
{
	D3DXVECTOR2 vec = bPos - cBrick::brickCentre;
	float dist = sqrt(vec.x*vec.x + vec.y*vec.y);
	return dist;
}

void cBrick::hitBrick()
{
	cD3DManager* d3dMgr = cD3DManager::getInstance();

	cBrick::hits--;

	if(cBrick::hits<=0)
	{
		alive=false;
	}
	else
	{
	std::ostringstream filepath;

	filepath << "sprites\\Brick_" << hits << ".png";

	cBrick::brickSprite.setTexture(d3dMgr->getTheD3DDevice(),filepath.str().c_str());
	}
	
}

bool cBrick::isAlive()
{
	return alive;
}

cBrick::~cBrick()
{
	brickSprite.~cSprite();
}

