#include "res_path.h"

#include <iostream>
#include <SDL.h>

#include "log_error.h"

std::string getResourcePath(const std::string& subDir) {

//Paths separated by \ in Windows, / elsewhere
#ifdef _WIN32
	const char PATH_SEP = '\\';
#else
	const char PATH_SEP = '/';
#endif

	//No need to call this more than once, keep it static.
	//Holds absolute path up until "res/".
	static std::string baseRes;

	if (baseRes.empty()) {
		char* basePath = SDL_GetBasePath();

		//SDL_GetBasePath() may return NULL if an error occured
		if (basePath != nullptr) {
			//Must free basePath
			baseRes = basePath;
			SDL_free(basePath);
		} else {
			logError(std::cout, "Error getting resource path.");
			logSDLError(std::cout, "SDL_GetBasePath");
			return "";
		}

		// Replace trailing "bin/" with "res/"
		size_t pos = baseRes.rfind("bin");
		baseRes = baseRes.substr(0, pos) + "res" + PATH_SEP;
	}

	return subDir.empty() ? (baseRes) : (baseRes + subDir + PATH_SEP);
}
