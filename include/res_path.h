#ifndef _RES_PATH_H
#define _RES_PATH_H

#include <string>

/**
 * Get the resource path for resoures located under "res/"
 * assuming the project directory has its binaries under "bin/".
 *
 * @param  subDir Optional subdirectory under "res/"
 * @return        Absolute path in UTF-8 encoding to "res/"
 */
std::string getResourcePath(const std::string &subDir = "");

#endif
