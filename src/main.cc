#include "main.h"

#include <iostream>
#include <sstream>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Dot.h"
#include "MyTexture.h"
#include "MyTimer.h"
#include "log_error.h"
#include "res_path.h"

bool init() {
	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		logSDLError(std::cout, "SDL_Init");
		return false;
	}

	/* Set Linear Texture Filtering */
	if (!SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		logError(std::cout, "Warning: Linear texture filtering not enabled.");
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

	/* Load Dot Texture */
	if (!gDotTexture.loadFromFile("dot.png")) {
		logError(std::cout, "Failed to load dot texture.");
		return false;
	}

	/* Load Background Texture */
	if (!gBGTexture.loadFromFile("levelbackground.png")) {
		logError(std::cout, "Failed to load background texture.");
		return false;
	}

	/* Load Red Texture */
	if (!gRedTexture.loadFromFile("particleRed.png")) {
		logError(std::cout, "Failed to load red particle texture.");
		return false;
	}
	/* Load Green Texture */
	if (!gGreenTexture.loadFromFile("particleGreen.png")) {
		logError(std::cout, "Failed to load green particle texture.");
		return false;
	}
	/* Load Blue Texture */
	if (!gBlueTexture.loadFromFile("particleBlue.png")) {
		logError(std::cout, "Failed to load blue particle texture.");
		return false;
	}
	/* Load Shimmer Texture */
	if (!gShimmerTexture.loadFromFile("particleShimmer.png")) {
		logError(std::cout, "Failed to load shimmer particle texture.");
		return false;
	}

	gRedTexture.setAlpha(192);
	gGreenTexture.setAlpha(192);
	gBlueTexture.setAlpha(192);
	gShimmerTexture.setAlpha(192);

	return true;
}

void close() {
	/* Free loaded textures */
	gFPSTextTexture.free();
	gDotTexture.free();
	gRedTexture.free();
	gGreenTexture.free();
	gBlueTexture.free();
	gShimmerTexture.free();
	gBGTexture.free();

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
	/* Variables */
	//Main loop flag
	bool quit = false;
	//Main event handler
	SDL_Event event;
	//Dot that will be moving around
	Dot dot;
	//Black text color
	SDL_Color textColor = {0, 0, 0, 255};
	//FPS timer
	MyTimer fpsTimer;
	//In-memory text stream
	std::stringstream timeText;
	//Start counting FPS
	int countedFrames = 0;
	fpsTimer.start();
	//Camera
	SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

	/* Main loop */
	while (!quit) {
		while (SDL_PollEvent(&event) != 0) {
			//User requests to quit
			//Either by closing window, or by pressing the esc key
			if (event.type == SDL_QUIT ||
					(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
				quit = true;
			}
			dot.handleEvent(event);
		}

		dot.move();

		//Center camera over dot
		camera.x = (dot.getPosX() + Dot::DOT_WIDTH / 2) - SCREEN_WIDTH / 2;
		camera.y = (dot.getPosY() + Dot::DOT_HEIGHT / 2) - SCREEN_HEIGHT / 2;

		//Keep camera in bounds
		if (camera.x < 0) {
			camera.x = 0;
		}
		if (camera.y < 0) {
			camera.y = 0;
		}
		if (camera.x > LEVEL_WIDTH - camera.w) {
			camera.x = LEVEL_WIDTH - camera.w;
		}
		if (camera.y > LEVEL_HEIGHT - camera.h) {
			camera.y = LEVEL_HEIGHT - camera.h;
		}

		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		//Calculate fps
		float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);

		//Set text to be rendered
		timeText.str("");
		timeText << avgFPS;

		//Load text
		if (!gFPSTextTexture.loadFromRenderedText(timeText.str().c_str(), textColor)) {
			logError(std::cout, "Unable to render FPS texture.");
		}

		//Render background, dot, text
		gBGTexture.render(0, 0, &camera);
		dot.render(camera.x, camera.y);
		gFPSTextTexture.render(SCREEN_WIDTH - gFPSTextTexture.getWidth(), 0);

		//Update screen
		SDL_RenderPresent(gRenderer);
		countedFrames++;
	}

	close();

	return 0;
}
