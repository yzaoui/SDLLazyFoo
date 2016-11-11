#include <SDL.h>
#include <iostream>

#include "log_error.h"
#include "main.h"
#include "res_path.h"

//The window we'll be rendering to
SDL_Window* gWindow = nullptr;

//The surface contained by the window
SDL_Surface* gScreenSurface = nullptr;

//The image we will load and show on the screen
SDL_Surface* gHelloWorld = nullptr;

bool init() {
	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		logSDLError(std::cout, "SDL_Init");
		return false;
	}

	/* Create Window */
	gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == nullptr) {
		logSDLError(std::cout, "SDL_CreateWindow");
		return false;
	}

	/* Get Window Surface */
	gScreenSurface = SDL_GetWindowSurface(gWindow);

	return true;
}

bool loadMedia() {
	/* Load Splash Image */
	std::string resPath = getResourcePath() + "hello_world.bmp";
	gHelloWorld = SDL_LoadBMP(resPath.c_str());
	if (gHelloWorld == nullptr) {
		logSDLError(std::cout, "SDL_LoadBMP");
		return false;
	}
	return true;
}

void close() {
	/* Deallocate Surface */
	SDL_FreeSurface(gHelloWorld);
	gHelloWorld = nullptr;

	/* Destroy Window */
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;

	/* Quit SDL Subsystems */
	SDL_Quit();
}

void logSDLError(std::ostream& os, const std::string& msg) {
	logError(os, msg + " error: " + SDL_GetError());
}

void logError(std::ostream& os, const std::string& msg) {
	os << msg << std::endl;
}

int main (int argc, char** argv) {
	/******************************
	 * Initialization
	 ******************************/
	if (!init()) {
		logError(std::cout, "Failed to initialize.");
		close();
		return 1;
	}
	/* Load Media */
	if (!loadMedia()) {
		logError(std::cout, "Failed to load media.");
		close();
		return 1;
	}
	//Apply the image
	SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
	//Update it
	SDL_UpdateWindowSurface(gWindow);
	//Wait two seconds
	SDL_Delay(2000);

	close();

	return 0;
}
