#ifndef _CSCENEMANAGER_H
#define _CSCENEMANAGER_H

#include "cSprite.h"
#include "cBrick.h"
#include "GameResources.h"
#include "GameConstants.h"

enum sceneID : int {MENU, LVL1, LVL2, LVL3, LVL4, LVL5, END};

class cSceneManager
{
private:
	static cSceneManager* mgrInstance;
	cSceneManager();
	cBrick lvl1Bricks[4];
	cBrick lvl2Bricks[4];
	cBrick lvl3Bricks[4];
	cBrick lvl4Bricks[4];
	cBrick lvl5Bricks[4];
	cBrick emptyArray[1];
	sceneID currentScene;
	LPDIRECT3DSURFACE9 aSurface;
public:
	static cSceneManager* getInstance();
	bool initializeScnMgr();
	sceneID getCurrentScene();
	LPDIRECT3DSURFACE9 getBackgroundSurface();
	int getBrickArrayLength();
	cBrick* getBrickArray();
	void nextScene(int game);
};

#endif