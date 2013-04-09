#ifndef _CBRICK_H
#define _CBRICK_H
#include "cSprite.h"

/*
====
Header file for the cBrick
====
*/

class cBrick : cSprite
{
private:
		int hits;					//Hit points remaining for the brick
		cSprite brickSprite;		//The 2d sprite for the brick
		D3DXVECTOR2 brickCentre;	//The centre of the brick's sprite
		bool alive;					//Variable determining if the brick has been hit or not
public:
	cBrick();					//Default constructor
	cBrick(D3DXVECTOR3 sPosition,  LPDIRECT3DDEVICE9 pd3dDevice, LPCSTR theFilename, int iHits);	//Constructor that takes position and hits
	~cBrick();						//Destructor
	cSprite getBrickSprite();		//Returns the brick's sprite
	int getBrickHits();				//Return the number of hits left for the brick
	void setBrickHits(int iHits);	//Manually set the hits on the brick
	float checkCollisionCoarse(D3DXVECTOR2 bPos);	//Checks for collisions
	void hitBrick();				//Hit the brick once
	D3DXVECTOR2 getBrickCentre();	//Returns the centre of the brick
	bool isAlive();					//Is the brick alive?
};

#endif