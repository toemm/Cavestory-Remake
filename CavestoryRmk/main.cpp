/* main.cpp
 * The entry point of the program
 * BACKUP 23-06-2016
 */

#include <SDL.h>
#include <SDL_image.h>
#include "globals.h"
#include "Game.h"

#include <iostream>

#define _CRTDBG_MAP_ALLOC

#include <iostream>
#include <crtdbg.h>

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif



int main(int argc, char *argv[]) {

	Game game;

	//_CrtDumpMemoryLeaks();


	return 0;
}

