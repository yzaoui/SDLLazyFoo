#ifndef _MAIN_H
#define _MAIN_H

#include <iostream>
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

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

#endif
