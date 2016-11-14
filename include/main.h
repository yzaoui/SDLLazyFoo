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
 * Frees surface and window, and shuts down SDL.
 */
void close();

/**
 * Loads an individual image as a texture.
 *
 * @param  path The path to the image.
 * @return      An SDL_Texture holding the image.
 */
SDL_Texture* loadTexture(std::string path);

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

//The currently displayed texture
SDL_Texture* gTexture = nullptr;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

#endif
