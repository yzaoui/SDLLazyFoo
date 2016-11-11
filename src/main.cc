#include <SDL.h>
#include <iostream>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/**
 * Log an SDL error with an error message to the output stream.
 *
 * @param os  The output stream to write the message to.
 * @param msg The error message to write, with format "{msg} error: {SDL_GetError()}".
 */
void logSDLError(std::ostream& os, const std::string& msg) {
	os << msg << " error: " << SDL_GetError() << std::endl;
}

int main (int argc, char** argv) {
	/******************************
	 * Initialization
	 ******************************/
	/* SDL initialization */
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		logSDLError(std::cout, "CreateWindow");
		SDL_Quit();
		return 1;
	}
	//Get window surface
	SDL_Surface* screenSurface = SDL_GetWindowSurface(window);
	//Fill it white
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
	//Update it
	SDL_UpdateWindowSurface(window);
	//Wait two seconds
	SDL_Delay(2000);

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}
