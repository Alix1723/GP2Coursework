/*
The player's paddle object
*/

#ifndef _CPADDLE_H
#define _CPADDLE_H

#include "cD3DManager.h"
#include "cD3DXSpriteMgr.h"
#include "cD3DXTexture.h"
#include "cSprite.h"

class cPaddle
{
private:

	D3DXVECTOR3 paddlePos;
	D3DXVECTOR2 paddlePos2D;
	D3DXVECTOR2 paddleTranslation;
	cD3DXTexture paddleSprite;
	D3DXVECTOR2 paddleCentre;
	D3DXMATRIX paddleMatrix;
public:
	
	cPaddle();			// Default constructor
	cPaddle(D3DXVECTOR3 sPosition,D3DXVECTOR2 sScale); // Constructor
	~cPaddle();			// Destructor
	D3DXVECTOR3 getPos();  // Return the paddle's current position
	void setPos(D3DXVECTOR3 sPosition); // set the position of the paddle
	D3DXVECTOR2 getPos2D();  // Return the paddle's current 2D position
	void setPos2D(D3DXVECTOR2 sPosition); // set the 2D position of the paddle
	void SetTranslation (D3DXVECTOR2 Translation);		// set the translation for the sprite
	D3DXMATRIX getMatrix();
	LPDIRECT3DTEXTURE9 getSpriteTex();
};

#endif