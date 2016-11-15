#ifndef _MAIN_H
#define _MAIN_H

#include <SDL.h>
#include <string>

#include "MyTexture.h"

/**
 * Starts up SD and creates a window.
 *
 * @return true on success, false on failure.
 */
bool init();

/**
 * Frees surface and window, and shuts down SDL.
 */
void close();

/**
 * Loads media.
 *
 * @return true on success, false on failure.
 */
bool loadMedia();



//The window we'll be rendering to
SDL_Window* gWindow = nullptr;

//The window's renderer
SDL_Renderer* gRenderer = nullptr;

//Arrow texture
MyTexture gArrowTexture;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

#endif
