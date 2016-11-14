#include "main.h"

#include <iostream>
#include <SDL_image.h>

#include "log_error.h"
#include "res_path.h"

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

	/* Initialize PNG loading */
	const int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		logIMGError(std::cout, "IMG_Init");
		return false;
	}

	/* Get Window Surface */
	gScreenSurface = SDL_GetWindowSurface(gWindow);

	return true;
}

bool loadMedia() {
	/* Load Splash Image */
	gImage = loadSurfaceFromImage(getResourcePath() + "scale.bmp");

	if (gImage == nullptr) {
		logError(std::cout, "Failed to load media.");
		return false;
	}

	return true;
}

void close() {
	/* Deallocate Surface */
	SDL_FreeSurface(gImage);
	gImage = nullptr;

	/* Destroy Window */
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;

	/* Quit SDL Subsystems */
	SDL_Quit();
}

SDL_Surface* loadSurfaceFromImage(std::string path) {
	//The final optimized image
	SDL_Surface* optimizedSurface = nullptr;

	//Load image from specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == nullptr) {
		logIMGError(std::cout, "IMG_Load");
	} else {
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface(loadedSurface,
			gScreenSurface->format, 0);

		if (optimizedSurface == nullptr) {
			logError(std::cout, "Unable to optimize image.");
			logSDLError(std::cout, "SDL_ConvertSurface");
		}

		//Free old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
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
		close();
		return 1;
	}

	/******************************
	 * Game Loop
	 ******************************/
	//Main loop flag
	bool quit = false;
	//Main event handler
	SDL_Event event;

	while (!quit) {
		while (SDL_PollEvent(&event) != 0) {
			//User requests to quit
			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}

		//Apply the stretched image
		SDL_Rect stretchRect;
		stretchRect.x = 0;
		stretchRect.y = 0;
		stretchRect.w = SCREEN_WIDTH;
		stretchRect.h = SCREEN_HEIGHT;
		SDL_BlitScaled(gImage, nullptr, gScreenSurface, &stretchRect);

		//Update the window surface
		SDL_UpdateWindowSurface(gWindow);
	}

	close();

	return 0;
}
