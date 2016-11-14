#ifndef _MAIN_H
#define _MAIN_H

#include <SDL.h>
#include <string>

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

/**
 * Loads an individual image.
 *
 * @param  path The path to the image.
 * @return      An SDL_Surface holding the image.
 */
SDL_Surface* loadSurfaceFromImage(std::string path);



//The window we'll be rendering to
SDL_Window* gWindow = nullptr;

//The surface contained by the window
SDL_Surface* gScreenSurface = nullptr;

//The image we will load and show on the screen
SDL_Surface* gImage = nullptr;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

#endif
