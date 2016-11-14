#include "log_error.h"

#include <SDL.h>

void logSDLError(std::ostream& os, const std::string& msg) {
	logError(os, msg + " error: " + SDL_GetError());
}

void logError(std::ostream& os, const std::string& msg) {
	os << msg << std::endl;
}
