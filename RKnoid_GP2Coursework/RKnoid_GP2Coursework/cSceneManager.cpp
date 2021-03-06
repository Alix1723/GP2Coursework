#include "cSceneManager.h"
#include "cD3DManager.h"

/*
====
Class file for the game's scene manager.
====
*/


cSceneManager* cSceneManager::mgrInstance = NULL;

//Default constructor.
cSceneManager::cSceneManager()
{

}

//Singleton pattern implementation.
cSceneManager* cSceneManager::getInstance()
{
	if(mgrInstance == NULL)
	{
		mgrInstance = new cSceneManager();
	}
	return cSceneManager::mgrInstance;
}

//Setup the initial state of the scene manager.
bool cSceneManager::initializeScnMgr()
{
	cD3DManager* d3dMgr = cD3DManager::getInstance();

	cSceneManager::currentScene = MENU;
	
	aSurface = d3dMgr->getD3DSurfaceFromFile("sprites\\menuBG.png");

	//Brick array sizes are extremely limited for performance reasons

	/*
	===
	LVL1
	===
	*/
	for (int i=0;i<2;i++)
		{
			for(int j=0;j<2;j++)
			{
				cSceneManager::lvl1Bricks[i+(j*2)] = cBrick(D3DXVECTOR3(50.0f+(i*50.0f),50.0f+(j*30.0f),0.0f),d3dMgr->getTheD3DDevice(),"sprites\\Brick_1.png",1);
			}
		}

	/*
	===
	LVL2
	===
	*/
	for (int i=0;i<2;i++)
		{
			for(int j=0;j<2;j++)
			{
				cSceneManager::lvl2Bricks[i+(j*2)] = cBrick(D3DXVECTOR3(50.0f+(i*50.0f),50.0f+(j*30.0f),0.0f),d3dMgr->getTheD3DDevice(),"sprites\\Brick_2.png",2);
			}
		}

	/*
	===
	LVL3
	===
	*/
	for (int i=0;i<2;i++)
		{
			for(int j=0;j<2;j++)
			{
				cSceneManager::lvl3Bricks[i+(j*2)] = cBrick(D3DXVECTOR3(50.0f+(i*50.0f),50.0f+(j*30.0f),0.0f),d3dMgr->getTheD3DDevice(),"sprites\\Brick_3.png",3);
			}
		}

	/*
	===
	LVL4
	===
	*/
	for (int i=0;i<2;i++)
		{
			for(int j=0;j<2;j++)
			{
					if(i==1)
					{cSceneManager::lvl4Bricks[i+(j*2)] = cBrick(D3DXVECTOR3(50.0f+(i*50.0f),50.0f+(j*30.0f),0.0f),d3dMgr->getTheD3DDevice(),"sprites\\Brick_1.png",1);}
					else
					{cSceneManager::lvl4Bricks[i+(j*2)] = cBrick(D3DXVECTOR3(50.0f+(i*50.0f),50.0f+(j*30.0f),0.0f),d3dMgr->getTheD3DDevice(),"sprites\\Brick_2.png",2);}
			}
		}
	/*
	===
	LVL5
	===
	*/
	for (int i=0;i<2;i++)
		{
			for(int j=0;j<2;j++)
			{
				cSceneManager::lvl5Bricks[i+(j*2)] = cBrick(D3DXVECTOR3(50.0f+(i*50.0f),50.0f+(j*30.0f),0.0f),d3dMgr->getTheD3DDevice(),"sprites\\Brick_3.png",3);
			}
		}


	return true;
}

//Return what scene is currently active
sceneID cSceneManager::getCurrentScene()
{
	return currentScene;
}

//Return the current background surface for the scene
LPDIRECT3DSURFACE9 cSceneManager::getBackgroundSurface()
{
	{
		return aSurface;
	}
}

//Get the array length for the scene's bricks (for looping through them)
int cSceneManager::getBrickArrayLength()
{
	if(currentScene==LVL1)
	{return 4;}
	else if(currentScene==LVL2)
	{return 4;}
	else if(currentScene==LVL3)
	{return 4;}
	else if(currentScene==LVL4)
	{return 4;}
	else if(currentScene==LVL5)
	{return 4;}
	else
	{return 0;}
}

//Returns this scene's arrangement of breakable bricks.
cBrick* cSceneManager::getBrickArray()
{
	if(currentScene==LVL1)
	{return lvl1Bricks;}
	else if(currentScene==LVL2)
	{return lvl2Bricks;}
	else if(currentScene==LVL3)
	{return lvl3Bricks;}
	else if(currentScene==LVL4)
	{return lvl4Bricks;}
	else if(currentScene==LVL5)
	{return lvl5Bricks;}
	else
	{return emptyArray;}
}

//Advance to the next scene; if it's the end, loop back to the beginning.
void cSceneManager::nextScene(int game)
{
	cD3DManager* d3dMgr = cD3DManager::getInstance();
	if(game==1||currentScene==LVL5)
	{
		cSceneManager::currentScene = END;
		aSurface = d3dMgr->getD3DSurfaceFromFile("sprites\\EndScreen.png");
	}
	else if(currentScene==END)
	{
		cSceneManager::currentScene = MENU;
		aSurface = d3dMgr->getD3DSurfaceFromFile("sprites\\menuBG.png");
	}
	else
	{	
		cSceneManager::currentScene = (sceneID)(cSceneManager::currentScene+1);
		aSurface = d3dMgr->getD3DSurfaceFromFile("sprites\\BG1.png");
	}
	
}
