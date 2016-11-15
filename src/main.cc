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
	/* Free Loaded Textures */
	gSpriteSheetTexture.free();

	/* Destroy Window */
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = nullptr;
	gWindow = nullptr;

	/* Quit SDL Subsystems */
	IMG_Quit();
	SDL_Quit();
}

bool loadMedia() {
	//Load sprite sheet texture
	if (!gSpriteSheetTexture.loadFromFile("sprites.png")) {
		logError(std::cout, "Failed to load sprite sheet texture.");
		return false;
	}

	if (!gRashu.loadFromFile("rashu.png")) {
		logError(std::cout, "Failed to load rashu texture.");
		return false;
	}

	if (!gFadeOutTexture.loadFromFile("fadeout.png")) {
		logError(std::cout, "Failed to load fade out texture.");
		return false;
	}
	gFadeOutTexture.setBlendMode(SDL_BLENDMODE_BLEND);

	if (!gFadeInTexture.loadFromFile("fadein.png")) {
		logError(std::cout, "Failed to load fade in texture.");
		return false;
	}

	for (int i = 0; i < 4; i++) {
		gSpriteClips[i] = {i*40, 0, 40, 40};
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
	//Modulation components
	Uint8 r = 255;
	Uint8 g = 255;
	Uint8 b = 255;
	Uint8 a = 255;

	while (!quit) {
		while (SDL_PollEvent(&event) != 0) {
			//User requests to quit
			//Either by closing window, or by pressing the esc key
			if (event.type == SDL_QUIT ||
					(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
				quit = true;

			//On keypress change RGB values
			} else if (event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
					case SDLK_e:
						r += 32;
						break;
					case SDLK_r:
						g += 32;
						break;
					case SDLK_t:
						b += 32;
						break;
					case SDLK_d:
						r -= 32;
						break;
					case SDLK_f:
						g -= 32;
						break;
					case SDLK_g:
						b -= 32;
						break;
					case SDLK_w:
						a = (a + 32 > 255 ? 255 : a + 32);
						break;
					case SDLK_s:
						a = (a - 32 < 0 ? 0 : a - 32);
						break;
				}
			}
		}

		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		gFadeInTexture.render(0, 0);

		//Modulate and render texture
		gSpriteSheetTexture.setColor(r, g, b);

		/* Really bad coordinate system */

		for (int i = 0; i < 16; i++) {
			gSpriteSheetTexture.render(i * 40, 440, &gSpriteClips[1]);
		}

		for (int i = 0; i < 9; i++) {
			gSpriteSheetTexture.render(200, 80 + i*40, &gSpriteClips[1]);
		}

		for (int i = 0; i < 9; i++) {
			gSpriteSheetTexture.render(400, 80 + i*40, &gSpriteClips[1]);
		}

		for (int i = 0; i < 4; i++) {
			gSpriteSheetTexture.render((6+i)*40, 80, &gSpriteClips[2]);
		}

		gRashu.render(300, 160, &gSpriteClips[0]);

		gFadeOutTexture.setAlpha(a);
		gFadeOutTexture.render(0, 0);

		//Update screen
		SDL_RenderPresent(gRenderer);
	}

	close();

	return 0;
}
