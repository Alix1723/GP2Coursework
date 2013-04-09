#include "cBrick.h"
#include "cD3DManager.h"
#include <sstream>

/*
====
Definition for the cBrick class.
====
*/

//Default brick constructor.
cBrick::cBrick()
{
	cBrick::brickSprite = cSprite();
	cBrick::hits = 1;
	cBrick::brickSprite.setSpriteCentre();
	cBrick::brickCentre = cBrick::brickSprite.getSpriteCentre();
	cBrick::alive = true;
}

//Create a brick with a specified position and number of hits.
cBrick::cBrick(D3DXVECTOR3 sPosition,  LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR theFilename, int iHits)
{
	cBrick::brickSprite = cSprite(sPosition,pd3dDevice,theFilename);
	cBrick::hits = iHits;
	cBrick::brickSprite.setSpriteCentre();
	cBrick::brickCentre = (D3DXVECTOR2)sPosition + D3DXVECTOR2(16,2);
	cBrick::alive = true;
}

//Return the sprite for the brick.
cSprite cBrick::getBrickSprite()
{
	return cBrick::brickSprite;
}

//Return how many hits the brick has left
int cBrick::getBrickHits()
{
	return cBrick::hits;
}

//Manually set the hits for a brick
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

//Return the centre point of a brick's sprite
D3DXVECTOR2 cBrick::getBrickCentre()
{
return cBrick::brickCentre;
}

//Check for the player's ball in a small radius for rough collision detection
float cBrick::checkCollisionCoarse(D3DXVECTOR2 bPos)
{
	D3DXVECTOR2 vec = bPos - cBrick::brickCentre;
	float dist = sqrt(vec.x*vec.x + vec.y*vec.y);
	return dist;
}

//Hit the brick, decreasing its hits by 1 and breaking it if it's out of hits
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

//Returns the state of the brick
bool cBrick::isAlive()
{
	return alive;
}

//Destructor
cBrick::~cBrick()
{
	brickSprite.~cSprite();
}

