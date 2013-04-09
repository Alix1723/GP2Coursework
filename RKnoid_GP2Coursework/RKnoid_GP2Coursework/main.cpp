
#include "GameResources.h"
#include "GameConstants.h"
#include "cD3DManager.h"
#include "cD3DXSpriteMgr.h"
#include "cD3DXTexture.h"
#include "cXAudio.h"
#include "cSprite.h"
#include "cPaddle.h"
#include "cBrick.h"
#include "cD3DXFont.h"
#include "cSceneManager.h"

using namespace std;

HINSTANCE hInst; // global handle to hold the application instance
HWND wndHandle; // global variable to hold the window handle

// Get a reference to the DirectX Manager
static cD3DManager* d3dMgr = cD3DManager::getInstance();

// Get a reference to the DirectX Sprite renderer Manager 
static cD3DXSpriteMgr* d3dxSRMgr = cD3DXSpriteMgr::getInstance();

//Get ref to the Scene Manager
static cSceneManager* sceneMgr = cSceneManager::getInstance();

//Game elements
static int pPoints = 0;
static int pLives = 3;

int brickArraySize;
int brickCounter=0;

float ballSpeed = 5.0f;
float xAxis = 0;

bool ballIsActive=false;
bool gEnd=false;

//Strings
char sPoints[25];
char sLives[25];
char sLevel[25];
char sLevShort[25];
int nLevelTime;

//Vectors
D3DXVECTOR2 paddleTranslate = D3DXVECTOR2(320,500);
D3DXVECTOR2 ballTranslate = D3DXVECTOR2(320,400);
D3DXVECTOR2 ballDirection = D3DXVECTOR2(ballSpeed,-ballSpeed);

//Sounds

cXAudio audSoundEffects;
cXAudio audBackgroundMusic;
//bool audioExists = true;

/*
==================================================================
* LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
* LPARAM lParam)
* The window procedure
==================================================================
*/

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Check any available messages from the queue
	switch (message)
	{
		case WM_KEYDOWN:
			{

				if(sceneMgr->getCurrentScene() == MENU)
				{
					if (wParam == VK_SPACE)
					{
						sceneMgr->nextScene(0);
						return 0;
					}
					return 0;
				}

				else if(sceneMgr->getCurrentScene() == END)
				{
					if (wParam == VK_SPACE)
					{//Reset everything
						gEnd=false;
						brickCounter=0;
						pPoints=0;
						pLives=3;
						sprintf_s( sPoints, 25, "POINTS: %d", pPoints);
						sprintf_s( sLives, 25, " LIVES: %d", pLives);
						sprintf_s( sLevel, 25, " LVL: %d", 1);
						sprintf_s( sLevShort, 25, "LEVEL %d!", 1);
						sceneMgr->initializeScnMgr();
						//sceneMgr->nextScene(0);
						return 0;
					}
					return 0;
				}

				else if (wParam == VK_SPACE&&!ballIsActive)
				{		
					//if(!audioExists){
							cXAudio audSoundEffects = cXAudio();
					//		audioExists=true;
					//	}
					ballIsActive=true;
					ballDirection.x = ballSpeed * (paddleTranslate.x-320)/350;
				//	if(audioExists){
						audSoundEffects.playSound(L"sounds\\boop.wav",false);//}
					return 0;
				}
				
				if(wParam == VK_ESCAPE)
				{
					PostQuitMessage(0);
					return 0;
				}

				return 0;
			}
		case WM_CLOSE:
			{
			// Exit the Game
				PostQuitMessage(0);
				 return 0;
			}

		case WM_DESTROY:
			{
				PostQuitMessage(0);
				return 0;
			}
	}
	// Always return the message to the default window
	// procedure for further processing
	return DefWindowProc(hWnd, message, wParam, lParam);
}

/*
==================================================================
* bool initWindow( HINSTANCE hInstance )
* initWindow registers the window class for the application, creates the window
==================================================================
*/
bool initWindow( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;
	// Fill in the WNDCLASSEX structure. This describes how the window
	// will look to the system
	wcex.cbSize = sizeof(WNDCLASSEX); // the size of the structure
	wcex.style = CS_HREDRAW | CS_VREDRAW; // the class style
	wcex.lpfnWndProc = (WNDPROC)WndProc; // the window procedure callback
	wcex.cbClsExtra = 0; // extra bytes to allocate for this class
	wcex.cbWndExtra = 0; // extra bytes to allocate for this instance
	wcex.hInstance = hInstance; // handle to the application instance
	wcex.hIcon = LoadIcon(hInstance,"sprites//ballIcon.ico"); // icon to associate with the application
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);// the default cursor
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); // the background color
	wcex.lpszMenuName = NULL; // the resource name for the menu
	wcex.lpszClassName = "RKNoid"; // the class name being created
	wcex.hIconSm = LoadIcon(hInstance,"sprites//ballIcon.ico"); // the handle to the small icon

	RegisterClassEx(&wcex);
	// Create the window
	wndHandle = CreateWindow("RKNoid",			// the window class to use
							 "RKNoid",			// the title bar text
							WS_OVERLAPPEDWINDOW,	// the window style
							CW_USEDEFAULT, // the starting x coordinate
							CW_USEDEFAULT, // the starting y coordinate
							700, // the pixel width of the window
							700, // the pixel height of the window
							NULL, // the parent window; NULL for desktop
							NULL, // the menu for the application; NULL for none
							hInstance, // the handle to the application instance
							NULL); // no values passed to the window
	// Make sure that the window handle that is created is valid
	if (!wndHandle)
		return false;
	// Display the window on the screen
	ShowWindow(wndHandle, SW_SHOW);
	UpdateWindow(wndHandle);
	
	return true;
}

/*
==================================================================
// This is winmain, the main entry point for Windows applications
==================================================================
*/
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
	// Initialize the window
	if ( !initWindow( hInstance ) )
		return false;
	// called after creating the window
	if ( !d3dMgr->initD3DManager(wndHandle) )
		return false;
	if ( !d3dxSRMgr->initD3DXSpriteMgr(d3dMgr->getTheD3DDevice()))
		return false;

	if( !sceneMgr->initializeScnMgr())
		return false;

	LPDIRECT3DSURFACE9 theBackbuffer = NULL;  // This will hold the back buffer

	cD3DXFont* pixFont = new cD3DXFont(d3dMgr->getTheD3DDevice(),hInstance, "pix");

	//Gameplay elements
	cSprite playerPaddle(D3DXVECTOR3(0.0f,0.0f,0.0f),d3dMgr->getTheD3DDevice(),"sprites\\Paddle.png");
	cSprite playerBall(D3DXVECTOR3(0.0f,0.0f,0.0f),d3dMgr->getTheD3DDevice(),"sprites\\Ball.png");

	RECT txtScorePos;
	RECT txtLivesPos;
	RECT txtLevelPos;
	RECT txtLevTimePos;
	RECT txtFinalScorePos;

	SetRect(&txtScorePos,10,630,500,700);
	SetRect(&txtLivesPos,465,630,700,700);
	SetRect(&txtLevelPos,465,550,700,700);
	SetRect(&txtLevTimePos,220,350,700,700);
	SetRect(&txtFinalScorePos,220,350,700,700);

	sprintf_s( sPoints, 25, "POINTS: %d", pPoints);
	sprintf_s( sLives, 25, " LIVES: %d", pLives);
	sprintf_s( sLevel, 25, " LVL: %d", 1);

	sprintf_s( sLevShort, 25, "LEVEL %d!", 1);

	playerPaddle.setSpriteCentre();
	playerBall.setSpriteCentre();

	__int64 freq = 0;
	float sPC = 1.0f/(float)freq;
	__int64 currentTime = 0;
	__int64 prevTime = 0;
	float numFrames = 0.0f;
	float timeElapsed = 0.0f;
	float frameRate = 1.0f/30.0f;

	audBackgroundMusic.playSound(L"sounds\\level1test.wav",true);	//BG music

	MSG msg;
	ZeroMemory( &msg, sizeof( msg ) );

	D3DXMATRIX paddleMatrix = playerPaddle.getSpriteTransformMatrix();
	D3DXMATRIX ballMatrix = playerBall.getSpriteTransformMatrix();
	 
	while( msg.message!=WM_QUIT )
	{
		// Check the message queue
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			//If scenemanager.etype != MENU or END
			sceneID currentScene = sceneMgr->getCurrentScene();
			/*
			===================
			THE MAIN MENU
			===================
			*/
			if(currentScene == MENU)
			{
				
				//Render
				d3dMgr->beginRender();
				theBackbuffer = d3dMgr->getTheBackBuffer();
				d3dMgr->updateTheSurface(sceneMgr->getBackgroundSurface(), theBackbuffer);
				d3dMgr->releaseTheBackbuffer(theBackbuffer);
				d3dMgr->endRender();
			}

			/*
			===================
			MAIN GAMEPLAY SCENES
			===================
			*/
			else if(currentScene == LVL1|| currentScene == LVL2||currentScene == LVL3||currentScene == LVL4||currentScene == LVL5)
			{
				//Set background surface
						
				//Move paddle left/right
				if(GetAsyncKeyState(VK_LEFT) - GetAsyncKeyState(VK_RIGHT)<0)
				{
					xAxis=-1;
				}
				else if(GetAsyncKeyState(VK_LEFT) - GetAsyncKeyState(VK_RIGHT)>0)
				{
					xAxis=1;
				}
				else
				{
					xAxis=0;
				}

				paddleTranslate.x += xAxis*paddleSpeed;

				QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
				float dt = (currentTime - prevTime)*sPC;
				timeElapsed +=dt;

				if(timeElapsed>frameRate){

				//Clamp paddle position to the window
				if(paddleTranslate.x<0){paddleTranslate.x=0;}
				if(paddleTranslate.x>700.0f - playerPaddle.getSTWidth()){paddleTranslate.x=700.0f - playerPaddle.getSTWidth();}

				//Ball collision detection
					//Left/right
					if(ballTranslate.x>700 - playerBall.getSTWidth()){ballDirection.x= -ballDirection.x; audSoundEffects.playSound(L"sounds\\beep.wav",false);}
					if(ballTranslate.x<0){ballDirection.x=-ballDirection.x; audSoundEffects.playSound(L"sounds\\beep.wav",false);}

					//Reset ball when it touches the bottom of the window
					if(ballTranslate.y>700 - playerBall.getSTHeight()){ //audSoundEffects.playSound(L"sounds\\feck.wav",false);
						ballDirection.y=-ballSpeed; ballIsActive=false; ballSpeed = 5; ballTranslate = D3DXVECTOR2(ballSpeed,-ballSpeed); 
					pLives--;
					pPoints-=50;
					sprintf_s( sLives, 25, "LIVES: %d", pLives);
					sprintf_s( sPoints, 25, "POINTS: %d", pPoints);
					//If the player's out of lives, end the game
						if(pLives<=0)
						{
							gEnd=true;
							break;
						}
					}

					//Top
					if(ballTranslate.y<0){ballDirection.y= -ballDirection.y; audSoundEffects.playSound(L"sounds\\beep.wav",false);}

				//Paddle
				if (ballTranslate.y>paddleTranslate.y-playerBall.getSTHeight() && ballTranslate.y<paddleTranslate.y && ballIsActive)
				{
					//A small horizontal tolerance for hitting the edge of the paddle
					float bTolerance = 40;

					if((ballTranslate.x+playerBall.getSTWidth()/2) > paddleTranslate.x - bTolerance && 
						ballTranslate.x+playerBall.getSTWidth()/2 < paddleTranslate.x+playerPaddle.getSTWidth()+bTolerance)
					{
						ballSpeed+=0.5;
						ballDirection.y=-ballSpeed;
						ballDirection.x = ballSpeed * ((ballTranslate.x+playerBall.getSTWidth()/2) - (paddleTranslate.x+playerPaddle.getSTWidth()/2))/100;
						audSoundEffects.playSound(L"sounds\\boop.wav",false);
					}
				}

				brickArraySize = sceneMgr->getBrickArrayLength();

				//Bricks collision check
				if(sceneMgr->getCurrentScene()!=END && ballIsActive && !gEnd){
					for(int b=0;b<brickArraySize;b++){
						if(sceneMgr->getBrickArray()[b].isAlive()){
							if(sceneMgr->getBrickArray()[b].checkCollisionCoarse(ballTranslate+D3DXVECTOR2(8,8)) < 30)
							{
								D3DXVECTOR2 tlc = sceneMgr->getBrickArray()[b].getBrickSprite().getSpritePos2D();
								if((ballTranslate.x+16) <  tlc.x || (ballTranslate.x) >  tlc.x+32)
								{
									ballDirection.x = ballDirection.x*-1;
								}
								else if((ballTranslate.y+16) <  tlc.y || (ballTranslate.y) >  tlc.y + 8)
								{
									ballDirection.y = ballDirection.y*-1;
						
								}
								sceneMgr->getBrickArray()[b].hitBrick();
								audSoundEffects.playSound(L"sounds\\hit.wav",false);
								//Breaking the brick
								if(!sceneMgr->getBrickArray()[b].isAlive()){
									pPoints+=100;
									brickCounter+=1;
									//Trigger next level
									if(brickCounter>=brickArraySize && !gEnd)
									{
										brickCounter=0;
										ballSpeed = 5;
										ballDirection.y=-ballSpeed; ballIsActive=false; ballTranslate = D3DXVECTOR2(ballSpeed,-ballSpeed); 
											audSoundEffects.playSound(L"sounds\\lvlComplete.wav",false);
											//Destroy and recreate the SFX player
											//audSoundEffects.~cXAudio();
											//audioExists=false;
											//Or don't because it arses everything up

										if(sceneMgr->getCurrentScene()==LVL5){
											gEnd=true;
											sceneMgr->nextScene(1);
										}
										else
										{
											sceneMgr->nextScene(0);
											paddleTranslate = D3DXVECTOR2(320,500);
											nLevelTime=100;
											sprintf_s( sLevShort, 25, "LEVEL %d!", (int)sceneMgr->getCurrentScene());
											sprintf_s( sLevel, 25, " LVL: %d", (int)sceneMgr->getCurrentScene());
										}
									}
								}
								//Take one hit off the brick
								else
								{
									pPoints+=10;
								}
								sprintf_s( sPoints, 25, "POINTS: %d", pPoints);
							}
						}
					}
				}

				if(nLevelTime>0){nLevelTime--;}

				//Attach ball to paddle before play
				if(ballIsActive){
					ballTranslate += ballDirection;
				}
				else
				{
					ballTranslate = paddleTranslate + D3DXVECTOR2(playerPaddle.getSTWidth()/2.0f - playerBall.getSTWidth()/2.0f,-21.0f);
				}

				D3DXMatrixTransformation2D(&paddleMatrix,NULL,NULL,NULL,NULL,NULL,&paddleTranslate);

				D3DXMatrixTransformation2D(&ballMatrix,NULL,NULL,NULL,NULL,NULL,&ballTranslate);
		
				playerPaddle.setSpritePos(D3DXVECTOR3(paddleMatrix._41,paddleMatrix._42,0));

				playerBall.setSpritePos(D3DXVECTOR3(ballMatrix._41,ballMatrix._42,0));

				//RENDERING
			
				d3dMgr->beginRender();

				theBackbuffer = d3dMgr->getTheBackBuffer();
				d3dMgr->updateTheSurface(sceneMgr->getBackgroundSurface(), theBackbuffer);
				d3dMgr->releaseTheBackbuffer(theBackbuffer);

				d3dxSRMgr->beginDraw();
			
					d3dxSRMgr->setTheTransform(paddleMatrix);
					d3dxSRMgr->drawSprite(playerPaddle.getTexture(),NULL,NULL,NULL,0xFFFFFFFF);

					d3dxSRMgr->setTheTransform(ballMatrix);
					d3dxSRMgr->drawSprite(playerBall.getTexture(),NULL,NULL,NULL,0xFFFFFFFF);
				
					//LAG WARNING FOR ARRAYS >8 IN SIZE
						for(int k=0;k<sceneMgr->getBrickArrayLength();k++){ 
						
							if(sceneMgr->getBrickArray()[k].isAlive()){
								d3dxSRMgr->setTheTransform(sceneMgr->getBrickArray()[k].getBrickSprite().getSpriteTransformMatrix());
								d3dxSRMgr->drawSprite(sceneMgr->getBrickArray()[k].getBrickSprite().getTexture(),NULL,NULL,NULL,0xFFFFFFFF);}
						}

				d3dxSRMgr->endDraw();
					//Text
					pixFont->printText(sPoints,txtScorePos);
					pixFont->printText(sLives,txtLivesPos);
					pixFont->printText(sLevel,txtLevelPos);
					if(nLevelTime>0){
						pixFont->printText(sLevShort,txtLevTimePos);
					}

				d3dMgr->endRender();
			
				timeElapsed=0.0f;
				
				}
			
				prevTime = currentTime;
				if(gEnd){sceneMgr->nextScene(1);}

				}
				/*
				===================
				THE END SCREEN
				===================
				*/
				if(sceneMgr->getCurrentScene() == END)
				{
								
					//Render
					d3dMgr->beginRender();
					theBackbuffer = d3dMgr->getTheBackBuffer();
					d3dMgr->updateTheSurface(sceneMgr->getBackgroundSurface(), theBackbuffer);
					d3dMgr->releaseTheBackbuffer(theBackbuffer);

					pixFont->printText(sPoints,txtFinalScorePos);
					d3dMgr->endRender();
					
				}
			}
		}
	d3dxSRMgr->cleanUp();
	d3dMgr->clean();

	return (int) msg.wParam;
}