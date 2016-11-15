#ifndef _MYTEXTURE_H
#define _MYTEXTURE_H

#include <SDL.h>
#include <string>

class MyTexture {
public:
	//Initialize members
	MyTexture();

	//Deallocate members
	~MyTexture();

	//Load image from specified path
	bool loadFromFile(std::string path);

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = nullptr);

	//Get image width
	int getWidth() const;

	//Get image height
	int getHeight() const;
private:
	//The actual hardware texture
	SDL_Texture* texture_;

	//Image width
	int width_;

	//Image height
	int height_;
};

#endif
