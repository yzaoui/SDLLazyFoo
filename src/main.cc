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
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
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
		return false;
	}

	/* Initialize SDL_mixer */
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
		logMixError(std::cout, "Mix_OpenAudio");
		return false;
	}

	return true;
}

bool loadMedia() {
	/* Open Font */
	const std::string resPath = getResourcePath();
	std::string path = resPath + "OpenSans-Regular.ttf";
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

	/* Load music controls texture */
	if (!gMusicControlsTexture.loadFromFile("prompt.png")) {
		logError(std::cout, "Failed to load music controls texture.");
		return false;
	}

	gRedTexture.setAlpha(192);
	gGreenTexture.setAlpha(192);
	gBlueTexture.setAlpha(192);
	gShimmerTexture.setAlpha(192);

	/* Load Music */
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

void close() {
	/* Free loaded textures */
	gFPSTextTexture.free();
	gDotTexture.free();
	gRedTexture.free();
	gGreenTexture.free();
	gBlueTexture.free();
	gShimmerTexture.free();
	gBGTexture.free();
	gMusicControlsTexture.free();

	/* Free sound effects */
	Mix_FreeChunk(gScratch);
	Mix_FreeChunk(gHigh);
	Mix_FreeChunk(gMedium);
	Mix_FreeChunk(gLow);
	gScratch = nullptr;
	gHigh = nullptr;
	gMedium = nullptr;
	gLow = nullptr;

	/* Free music */
	Mix_FreeMusic(gMusic);
	gMusic = nullptr;

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
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char** argv) {
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
					default:
						break;
				}
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
		if (!gFPSTextTexture.loadFromRenderedText(timeText.str(), textColor)) {
			logError(std::cout, "Unable to render FPS texture.");
		}

		//Render background, dot, text
		gBGTexture.render(0, 0, &camera);
		gMusicControlsTexture.render(100 - camera.x, 100 - camera.y);
		dot.render(camera.x, camera.y);
		gFPSTextTexture.render(SCREEN_WIDTH - gFPSTextTexture.getWidth(), 0);

		//Update screen
		SDL_RenderPresent(gRenderer);
		countedFrames++;
	}

	close();

	return 0;
}
