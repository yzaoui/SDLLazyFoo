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
	gWindow = SDL_CreateWindow("SDL Testing", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == nullptr) {
		logSDLError(std::cout, "SDL_CreateWindow");
		return false;
	}

	/* Create renderer for window */
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == nullptr) {
		logSDLError(std::cout, "SDL_CreateRenderer");
		return false;
	}

	/* Initialize renderer color */
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);	//RGBa, 8-bit ints

	/* Initialize PNG loading */
	const int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		logIMGError(std::cout, "IMG_Init");
		return false;
	}

	return true;
}

void close() {
	/* Free Loaded Texture */
	SDL_DestroyTexture(gTexture);
	gTexture = nullptr;

	/* Destroy Window */
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = nullptr;
	gWindow = nullptr;

	/* Quit SDL Subsystems */
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(std::string path) {
	//The final texture
	SDL_Texture* newTexture = nullptr;

	//Load image from specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == nullptr) {
		logIMGError(std::cout, "IMG_Load");
	} else {
		//Create texture from surface
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == nullptr) {
			logSDLError(std::cout, "SDL_CreateTextureFromSurface");
		}

		//Free old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

bool loadMedia() {
	/* Load Splash Image */
	gTexture = loadTexture(getResourcePath() + "viewport.png");

	if (gTexture == nullptr) {
		logError(std::cout, "Failed to load media.");
		return false;
	}

	return true;
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
			//Either by closing window, or by pressing the esc key
			if (event.type == SDL_QUIT ||
					event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
				quit = true;
			}
		}

		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		//Top-left viewport
		SDL_Rect topLeftViewport = {0, 0,
			SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
		SDL_RenderSetViewport(gRenderer, &topLeftViewport);
		//Render texture to screen
		SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);

		//Top-right viewport
		SDL_Rect topRightViewport = {SCREEN_WIDTH / 2, 0,
			SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
		SDL_RenderSetViewport(gRenderer, &topRightViewport);
		//Render texture to screen
		SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);

		//Bottom viewport
		SDL_Rect bottomViewport = {0, SCREEN_HEIGHT / 2,
			SCREEN_WIDTH, SCREEN_HEIGHT / 2};
		SDL_RenderSetViewport(gRenderer, &bottomViewport);
		//Render texture to screen
		SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);

		//Update screen
		SDL_RenderPresent(gRenderer);
	}

	close();

	return 0;
}
