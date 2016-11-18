#include "main.h"

#include <iostream>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "log_error.h"
#include "res_path.h"

bool init() {
	/* Initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
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

	/* Initialize SDL_mixer */
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
		logMixError(std::cout, "Mix_OpenAudio");
		return false;
	}

	return true;
}

void close() {
	/* Free loaded image */
	gPromptTexture.free();

	/* Free Sound Effects */
	Mix_FreeChunk(gScratch);
	Mix_FreeChunk(gHigh);
	Mix_FreeChunk(gMedium);
	Mix_FreeChunk(gLow);
	gScratch = nullptr;
	gHigh = nullptr;
	gMedium = nullptr;
	gLow = nullptr;

	/* Free Music */
	Mix_FreeMusic(gMusic);
	gMusic = nullptr;

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

bool loadMedia() {
	/* Load Prompt Texture */
	if (!gPromptTexture.loadFromFile("prompt.png")) {
		logError(std::cout, "Failed to load prompt texture.");
		return false;
	}

	/* Load Music */
	const std::string resPath = getResourcePath();
	std::string path;
	path = resPath + "beat.wav";
	gMusic = Mix_LoadMUS(path.c_str());
	if (gMusic == nullptr) {
		logMixError(std::cout, "Mix_LoadMUS");
		return false;
	}

	/* Load Sound Effects */
	path = resPath + "scratch.wav";
	gScratch = Mix_LoadWAV(path.c_str());
	if (gScratch == nullptr) {
		logMixError(std::cout, "Mix_LoadWAV");
		return false;
	}

	path = resPath + "high.wav";
	gHigh = Mix_LoadWAV(path.c_str());
	if (gHigh == nullptr) {
		logMixError(std::cout, "Mix_LoadWAV");
		return false;
	}

	path = resPath + "medium.wav";
	gMedium = Mix_LoadWAV(path.c_str());
	if (gMedium == nullptr) {
		logMixError(std::cout, "Mix_LoadWAV");
		return false;
	}

	path = resPath + "low.wav";
	gLow = Mix_LoadWAV(path.c_str());
	if (gLow == nullptr) {
		logMixError(std::cout, "Mix_LoadWAV");
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
			} else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					//Play high sound effect
					case SDLK_1:
						//Arguments:
						//	-Channel, -1 for nearest available channel
						//	-Sound effect
						//	-Number of times to repeat per button press
						Mix_PlayChannel(-1, gHigh, 0);
						break;

					//Play medium sound effect
					case SDLK_2:
						Mix_PlayChannel(-1, gMedium, 0);
						break;

					//Play low sound effect
					case SDLK_3:
						Mix_PlayChannel(-1, gLow, 0);
						break;

					//Play scratch sound effect
					case SDLK_4:
						Mix_PlayChannel(-1, gScratch, 0);
						break;

					case SDLK_9:
						//If no music is playing
						if (Mix_PlayingMusic() == 0) {
							//Play the music
							Mix_PlayMusic(gMusic, -1);
						} else {
							//If music is paused
							if (Mix_PausedMusic() == 1) {
								//Resume music
								Mix_ResumeMusic();
							} else {
								//Pause music
								Mix_PauseMusic();
							}
						}
						break;

					case SDLK_0:
					//Stop music
					Mix_HaltMusic();
					break;
				}
			}
		}

		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		//Render prompt
		gPromptTexture.render(0, 0);

		//Update screen
		SDL_RenderPresent(gRenderer);
	}

	close();

	return 0;
}
