#include "MyTexture.h"

#include <iostream>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "log_error.h"
#include "res_path.h"

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

void MyTexture::load(SDL_Surface* surface) {
	//Create texture from surface
	texture_ = SDL_CreateTextureFromSurface(gRenderer, surface);

	if (texture_ == nullptr) {
		logSDLError(std::cout, "SDL_CreateTextureFromSurface");
	} else {
		width_ = surface->w;
		height_ = surface->h;
	}

	//Free old surface
	SDL_FreeSurface(surface);
}

bool MyTexture::loadFromFile(std::string path) {
	free();

	path = getResourcePath() + path;

	//Load image from specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == nullptr) {
		logIMGError(std::cout, "IMG_Load");
	} else {
		load(loadedSurface);
	}

	return (texture_ != nullptr);

}

bool MyTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor) {
	/*free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);

	if (textSurface == nullptr) {
		logTTFError(std::cout, "TTF_RenderText_Solid");
	} else {
		load(textSurface);
	}*/

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

void MyTexture::setBlendMode(SDL_BlendMode blending) {
	SDL_SetTextureBlendMode(texture_, blending);
}

void MyTexture::setAlpha(Uint8 alpha) {
	SDL_SetTextureAlphaMod(texture_, alpha);
}

void MyTexture::render(int x, int y, SDL_Rect* clip, double angle,
	SDL_Point* center, SDL_RendererFlip flip) {

	//Set rendering space and render to screen
	SDL_Rect renderQuad = {x, y, width_, height_};

	//Set clip rendering dimensions
	if (clip != nullptr) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, texture_, clip, &renderQuad,
		angle, center, flip);
}

int MyTexture::getWidth() const {
	return width_;
}

int MyTexture::getHeight() const {
	return height_;
}
