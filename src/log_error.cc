#include "log_error.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

void logSDLError(std::ostream& os, const std::string& msg) {
	logError(os, msg + " error: " + SDL_GetError());
}

void logIMGError(std::ostream& os, const std::string& msg) {
	logError(os, msg + " error: " + IMG_GetError());
}

void logTTFError(std::ostream& os, const std::string& msg) {
	logError(os, msg + " error: " + TTF_GetError());
}

void logError(std::ostream& os, const std::string& msg) {
	SDL_Log(msg.c_str());
}
