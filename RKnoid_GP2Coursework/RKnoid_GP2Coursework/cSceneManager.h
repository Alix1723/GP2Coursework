#ifndef _CSCENEMANAGER_H
#define _CSCENEMANAGER_H

/*
====
Header file for cSceneManager
====
*/

#include "cSprite.h"
#include "cBrick.h"
#include "GameResources.h"
#include "GameConstants.h"

enum sceneID : int {MENU, LVL1, LVL2, LVL3, LVL4, LVL5, END};		//Enumerated type for each scene

class cSceneManager
{
private:
	static cSceneManager* mgrInstance;
	cSceneManager();			
	cBrick lvl1Bricks[4];		//Arrays containing the bricks for gameplay
	cBrick lvl2Bricks[4];
	cBrick lvl3Bricks[4];
	cBrick lvl4Bricks[4];
	cBrick lvl5Bricks[4];
	cBrick emptyArray[1];		//Blank array as a shortcut for the End scene
	sceneID currentScene;		//The currently loaded scene
	LPDIRECT3DSURFACE9 aSurface;	//The 2D surface
public:
	static cSceneManager* getInstance();			//Gets a reference to the scene manager
	bool initializeScnMgr();						//Initialize the scene manager
	sceneID getCurrentScene();						//Return the current scene
	LPDIRECT3DSURFACE9 getBackgroundSurface();		//Return the current background surface
	int getBrickArrayLength();						//Get the current brick array's size
	cBrick* getBrickArray();						//Return's the current scene's array of bricks
	void nextScene(int game);						//Advance to the next scene
};

#endif