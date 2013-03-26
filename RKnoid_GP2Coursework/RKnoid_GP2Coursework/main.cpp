
#include "GameConstants.h"
#include "cD3DManager.h"
#include "cD3DXSpriteMgr.h"
#include "cD3DXTexture.h"
#include "cSprite.h"
#include "cPaddle.h"

using namespace std;

HINSTANCE hInst; // global handle to hold the application instance
HWND wndHandle; // global variable to hold the window handle

// Get a reference to the DirectX Manager
static cD3DManager* d3dMgr = cD3DManager::getInstance();

// Get a reference to the DirectX Sprite renderer Manager 
static cD3DXSpriteMgr* d3dxSRMgr = cD3DXSpriteMgr::getInstance();
	
D3DXVECTOR2 paddleTranslate = D3DXVECTOR2(320,500);
D3DXVECTOR2 ballTranslate = D3DXVECTOR2(320,400);

bool ballIsActive=false;


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
				if (wParam == VK_SPACE&&!ballIsActive)
				{		
					ballIsActive=true;
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

	LPDIRECT3DSURFACE9 aSurface;				// the Direct3D surface
	LPDIRECT3DSURFACE9 theBackbuffer = NULL;  // This will hold the back buffer
	
	//Creates a new Paddle object
	//cPaddle playerPaddle(D3DXVECTOR3(300.0f,300.0f,0.0f),D3DXVECTOR2(1.0f,1.0f));

	cSprite playerPaddle(D3DXVECTOR3(0.0f,0.0f,0.0f),d3dMgr->getTheD3DDevice(),"sprites\\Paddle.png");

	cSprite playerBall(D3DXVECTOR3(0.0f,0.0f,0.0f),d3dMgr->getTheD3DDevice(),"sprites\\Ball.png");

	playerPaddle.setSpriteCentre();

	playerBall.setSpriteCentre();
	
	float ballSpeed = 5;

	D3DXVECTOR2 ballDirection = D3DXVECTOR2(ballSpeed,-ballSpeed);

	MSG msg;
	ZeroMemory( &msg, sizeof( msg ) );

	D3DXMATRIX paddleMatrix = playerPaddle.getSpriteTransformMatrix();

	D3DXMATRIX ballMatrix = playerBall.getSpriteTransformMatrix();

	// Create the background surface
	aSurface = d3dMgr->getD3DSurfaceFromFile("sprites\\BG1.png");

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
			//Move paddle left/right
			paddleTranslate.x += (GetAsyncKeyState(VK_LEFT) - GetAsyncKeyState(VK_RIGHT))/2000;


			//Clamp paddle position to the window
			if(paddleTranslate.x<0){paddleTranslate.x=0;}
			if(paddleTranslate.x>700 - playerPaddle.getSTWidth()){paddleTranslate.x=700 - playerPaddle.getSTWidth();}

			//Ball collision detection
			//Walls
			if(ballTranslate.x>700 - playerBall.getSTWidth()){ballDirection.x=-ballSpeed;}
			if(ballTranslate.x<0){ballDirection.x=ballSpeed;}

			if(ballTranslate.y>700 - playerBall.getSTHeight()){//Reset ball
				ballDirection.y=-ballSpeed; ballIsActive=false; ballSpeed = 5; ballTranslate = D3DXVECTOR2(ballSpeed,-ballSpeed);}

			if(ballTranslate.y<0){ballDirection.y=ballSpeed;}

			//Paddle
			if (ballTranslate.y>paddleTranslate.y-playerBall.getSTHeight() && ballTranslate.y<paddleTranslate.y && ballIsActive)
			{
				float bTolerance = 40;

				if((ballTranslate.x+playerBall.getSTWidth()/2) > paddleTranslate.x - bTolerance && 
					ballTranslate.x+playerBall.getSTWidth()/2 < paddleTranslate.x+playerPaddle.getSTWidth()+bTolerance)
				{
					ballSpeed+=0.5;
					ballDirection.y=-ballSpeed;
					
				}
			}

			//Attach ball to paddle before play
			if(ballIsActive){
				ballTranslate += ballDirection;
			}

			else
			{
				ballTranslate = paddleTranslate + D3DXVECTOR2(playerPaddle.getSTWidth()/2 - playerBall.getSTWidth()/2,-21);
			}


			D3DXMatrixTransformation2D(&paddleMatrix,NULL,NULL,NULL,NULL,NULL,&paddleTranslate);

			D3DXMatrixTransformation2D(&ballMatrix,NULL,NULL,NULL,NULL,NULL,&ballTranslate);
			
			playerPaddle.setSpritePos(D3DXVECTOR3(paddleMatrix._41,paddleMatrix._42,0));

			playerBall.setSpritePos(D3DXVECTOR3(ballMatrix._41,ballMatrix._42,0));

			//RENDERING
			d3dMgr->beginRender();

			theBackbuffer = d3dMgr->getTheBackBuffer();

			d3dMgr->updateTheSurface(aSurface, theBackbuffer);

			d3dMgr->releaseTheBackbuffer(theBackbuffer);

			d3dxSRMgr->beginDraw();
			
				d3dxSRMgr->setTheTransform(paddleMatrix);

				d3dxSRMgr->drawSprite(playerPaddle.getTexture(),NULL,NULL,NULL,0xFFFFFFFF);

				d3dxSRMgr->setTheTransform(ballMatrix);

				d3dxSRMgr->drawSprite(playerBall.getTexture(),NULL,NULL,NULL,0xFFFFFFFF);
			
			
			d3dxSRMgr->endDraw();

			d3dMgr->endRender();
		}
	}
	d3dxSRMgr->cleanUp();
	d3dMgr->clean();
	return (int) msg.wParam;
}
