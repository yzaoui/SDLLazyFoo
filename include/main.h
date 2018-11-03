#ifndef _MAIN_H
#define _MAIN_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>

#include "MyTexture.h"

/**
 * Starts up SD and creates a window.
 *
 * @return true on success, false on failure.
 */
bool init();

/**
 * Loads media.
 *
 * @return true on success, false on failure.
 */
bool loadMedia();

/**
 * Frees surface and window, and shuts down SDL.
 */
void close();

//The window we'll be rendering to
SDL_Window* gWindow = nullptr;

//The window's renderer
SDL_Renderer* gRenderer = nullptr;

//The global font
TTF_Font* gFont;

//The FPS texture
MyTexture gFPSTextTexture;
//The background texture
MyTexture gBGTexture;
//The dot texture
MyTexture gDotTexture;
//The particle textures
MyTexture gRedTexture;
MyTexture gGreenTexture;
MyTexture gBlueTexture;
MyTexture gShimmerTexture;
//Music controls image
MyTexture gMusicControlsTexture;
//Music
Mix_Music* gMusic = nullptr;
//Sound effects
Mix_Chunk* gScratch = nullptr;
Mix_Chunk* gHigh = nullptr;
Mix_Chunk* gMedium = nullptr;
Mix_Chunk* gLow = nullptr;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Level dimension constants
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

#endif
