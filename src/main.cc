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
	gRenderer = SDL_CreateRenderer(gWindow, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

	/* Initialize SDL_ttf */
	if (TTF_Init() == -1) {
		logTTFError(std::cout, "TTF_Init");
	}

	return true;
}

void close() {
	/* Free Loaded Textures */
	gTextTexture.free();

	/* Free Global Font */
	TTF_CloseFont(gFont);
	gFont = nullptr;

	/* Destroy Window */
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = nullptr;
	gWindow = nullptr;

	/* Quit SDL Subsystems */
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool loadMedia() {
	//Open the font
	std::string path = getResourcePath() + "OpenSans-Regular.ttf";
	gFont = TTF_OpenFont(path.c_str(), 28);

	if (gFont == nullptr) {
		logTTFError(std::cout, "TTF_OpenFont");
		return false;
	}

	//Render text
	SDL_Color textColor = {0, 0, 0}; //RGBa
	if (!gTextTexture.loadFromRenderedText(
		"The quick brown fox jumps over the lazy dog", textColor)) {
		logError(std::cout, "Failed to render text texture.");
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
					(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
				quit = true;
			}
		}

		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		//Render text
		gTextTexture.render(0, 0);

		//Update screen
		SDL_RenderPresent(gRenderer);
	}

	close();

	return 0;
}
