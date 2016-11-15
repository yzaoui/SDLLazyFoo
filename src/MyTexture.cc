#include "MyTexture.h"

#include <iostream>
#include <SDL_image.h>

#include "log_error.h"

//Do this better later on
extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;

MyTexture::MyTexture() {
	texture_ = nullptr;
	width_ = 0;
	height_ = 0;
}

MyTexture::~MyTexture() {
	free();
}

bool MyTexture::loadFromFile(std::string path) {
	free();

	//The final texture
	SDL_Texture* newTexture = nullptr;

	//Load image from specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == nullptr) {
		logIMGError(std::cout, "IMG_Load");
	} else {
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE,
			SDL_MapRGB(loadedSurface->format, 0xFF, 0, 0xFF));

		//Create texture from surface
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == nullptr) {
			logSDLError(std::cout, "SDL_CreateTextureFromSurface");
		} else {
			width_ = loadedSurface->w;
			height_ = loadedSurface->h;
		}

		//Free old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	texture_ = newTexture;

	return (texture_ != nullptr);
}

void MyTexture::free() {
	//Free texture if it exists
	if (texture_ != nullptr) {
		SDL_DestroyTexture(texture_);
		texture_ = nullptr;
		width_ = 0;
		height_ = 0;
	}
}

void MyTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	//Modulate texture
	SDL_SetTextureColorMod(texture_, red, green, blue);
}

void MyTexture::render(int x, int y, SDL_Rect* clip) {
	//Set rendering space and render to screen
	SDL_Rect renderQuad = {x, y, width_, height_};

	//Set clip rendering dimensions
	if (clip != nullptr) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopy(gRenderer, texture_, clip, &renderQuad);
}

int MyTexture::getWidth() const {
	return width_;
}

int MyTexture::getHeight() const {
	return height_;
}
