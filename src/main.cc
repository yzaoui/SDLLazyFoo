#include "main.h"

#include <iostream>
#include <sstream>
#include <SDL_image.h>
#include <SDL_ttf.h>

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

bool loadMedia() {
	/* Open Font */
	std::string path = getResourcePath() + "OpenSans-Regular.ttf";
	gFont = TTF_OpenFont(path.c_str(), 28);
	if (gFont == nullptr) {
		logTTFError(std::cout, "TTF_OpenFont");
		return false;
	}

	/* Set Text Color */
	SDL_Color textColor = {0, 0, 0, 255};

	/* Load Prompt Texture */
	if (!gPromptTextTexture.loadFromRenderedText(
		"Press Enter to Reset Start Timer.", textColor)) {
		logError(std::cout, "Failed to render prompt texture.");
		return false;
	}

	return true;
}

void close() {
	/* Free loaded textures */
	gPromptTextTexture.free();
	gTimeTextTexture.free();

	/* Free Global Font */
	TTF_CloseFont(gFont);
	gFont = nullptr;

	/* Destroy Window */
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = nullptr;
	gWindow = nullptr;

	/* Quit SDL Subsystems */
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
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
	//Black text color
	SDL_Color textColor = {0, 0, 0, 255};
	//Current time
	Uint32 startTime = 0;
	//In-memory text stream
	std::stringstream timeText;

	while (!quit) {
		while (SDL_PollEvent(&event) != 0) {
			//User requests to quit
			//Either by closing window, or by pressing the esc key
			if (event.type == SDL_QUIT ||
					(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
				quit = true;
			} else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
				startTime = SDL_GetTicks();
			}
		}

		//Set text to be rendered
		timeText.str("");
		timeText << "Milliseconds since start time " << SDL_GetTicks() - startTime;

		//Render text
		if (!gTimeTextTexture.loadFromRenderedText(timeText.str().c_str(), textColor)) {
			logError(std::cout, "Unable to render time texture.");
		}

		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		//Render textures
		gPromptTextTexture.render(0, 0);
		gTimeTextTexture.render(100, 100);

		//Update screen
		SDL_RenderPresent(gRenderer);
	}

	close();

	return 0;
}
