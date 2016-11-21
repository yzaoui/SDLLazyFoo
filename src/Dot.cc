#include "Dot.h"

#include "MyTexture.h"

extern const int SCREEN_WIDTH = 640;
extern const int SCREEN_HEIGHT = 480;
extern MyTexture gDotTexture;

Dot::Dot() : posX_(0), posY_(0), velX_(0), velY_(0) {}

void Dot::handleEvent(SDL_Event& e) {
	//If key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		//Adjust velocity
		switch (e.key.keysym.sym) {
			case SDLK_UP:
				velY_ -= DOT_VEL;
				break;
			case SDLK_DOWN:
				velY_ += DOT_VEL;
				break;
			case SDLK_LEFT:
				velX_ -= DOT_VEL;
				break;
			case SDLK_RIGHT:
				velX_ += DOT_VEL;
				break;
		}
	//If key was released
	} else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
		//Adjust velocity
		switch (e.key.keysym.sym) {
			case SDLK_UP:
				velY_ += DOT_VEL;
				break;
			case SDLK_DOWN:
				velY_ -= DOT_VEL;
				break;
			case SDLK_LEFT:
				velX_ += DOT_VEL;
				break;
			case SDLK_RIGHT:
				velX_ -= DOT_VEL;
				break;
		}
	}
}

void Dot::move() {
	//Move dot left or right
	posX_ += velX_;

	//If dot went too far left or right
	if ((posX_ < 0) || (posX_ + DOT_WIDTH > SCREEN_WIDTH)) {
		//Move back
		posX_ -= velX_;
	}

	posY_ += velY_;

	if ((posY_ < 0) || (posY_ + DOT_WIDTH > SCREEN_HEIGHT)) {
		posY_ -= velY_;
	}
}

void Dot::render() {
	gDotTexture.render(posX_, posY_);
}
