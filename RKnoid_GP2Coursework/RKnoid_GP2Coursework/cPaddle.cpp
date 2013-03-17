/*
Player's paddle
*/
#include "cPaddle.h"

// Get a reference to the DirectX Manager
static cD3DManager* d3dMgr = cD3DManager::getInstance();

// Get a reference to the DirectX Sprite renderer Manager 
static cD3DXSpriteMgr* d3dxSRMgr = cD3DXSpriteMgr::getInstance();


cPaddle::cPaddle()
{
	cPaddle::paddlePos.x = 100.0f;
	cPaddle::paddlePos.y = 100.0f;
	cPaddle::paddlePos.z = 0.0f;
	
	cSprite paddleSprite(cPaddle::paddlePos,d3dMgr->getTheD3DDevice(),"sprites\\Paddle.png");
	
	paddleSprite.setSpriteCentre();
}

cPaddle::cPaddle(D3DXVECTOR3 sPosition, D3DXVECTOR2 sScale)
{
	cPaddle::paddlePos = sPosition;

	cSprite paddleTexture(cPaddle::paddlePos,d3dMgr->getTheD3DDevice(),"sprites\\Paddle.png");
	paddleTexture.setSpriteCentre();
	
}

cPaddle::~cPaddle()			
{
}

D3DXVECTOR3 cPaddle::getPos()
{
	return cPaddle::paddlePos;
}

void cPaddle::setPos(D3DXVECTOR3 pPos)
{
	paddleTranslation = D3DXVECTOR2(pPos - paddlePos);

	D3DXVECTOR2 paddleScale = D3DXVECTOR2(1,1);
	paddlePos = pPos;
	paddlePos2D = D3DXVECTOR2(pPos);
	D3DXMatrixTransformation2D(&paddleMatrix , NULL , 0.0f ,& paddleScale ,& (paddleCentre) , NULL, & paddleTranslation);
}

void cPaddle::setPos2D(D3DXVECTOR2 pPos)
{
	paddleTranslation = pPos - D3DXVECTOR2(paddlePos);
	 	
	D3DXVECTOR2 paddleScale = D3DXVECTOR2(1,1);
	paddlePos = D3DXVECTOR3(pPos);
	paddlePos2D = pPos;
	D3DXMatrixTransformation2D(&paddleMatrix , NULL , 0.0f ,& paddleScale ,& (paddleCentre) , NULL, &paddleTranslation);
}

void cPaddle::SetTranslation (D3DXVECTOR2 Translation)
{

}

D3DXMATRIX cPaddle::getMatrix()
{
	return paddleMatrix;
}

LPDIRECT3DTEXTURE9 cPaddle::getSpriteTex()
{
	return paddleSprite.getTexture();
}