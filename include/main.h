#ifndef _MAIN_H
#define _MAIN_H

#include <SDL.h>
#include <SDL_mixer.h>
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

//The prompt texture
MyTexture gPromptTexture;

//The music that will be played
Mix_Music* gMusic = nullptr;

//The sound effects that will be used
Mix_Chunk* gScratch = nullptr;
Mix_Chunk* gHigh = nullptr;
Mix_Chunk* gMedium = nullptr;
Mix_Chunk* gLow = nullptr;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

#endif
