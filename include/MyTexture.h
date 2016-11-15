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

	//Load image from specified path
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = nullptr,
		double angle = 0.0, SDL_Point* center = nullptr,
		SDL_RendererFlip flip = SDL_FLIP_NONE);

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

	void load(SDL_Surface* surface);
};

#endif
