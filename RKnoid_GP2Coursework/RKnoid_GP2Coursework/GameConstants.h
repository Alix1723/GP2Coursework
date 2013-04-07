#ifndef _GAMECONSTANTS_H
#define _GAMECONSTANTS_H

// Include the Windows header file that’s needed for all Windows applications
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <XAudio2.h>
#include "SDKwavefile.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <strsafe.h>


//Game constants
const int rows = 4;
const int columns = 12;
const int brickArraySize = rows*columns;
const int paddleSpeed = 10;


#endif