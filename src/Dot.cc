#include "Dot.h"

#include "helpers.h"
#include "MyTexture.h"

extern const int LEVEL_WIDTH = 1280;
extern const int LEVEL_HEIGHT = 960;
extern MyTexture gDotTexture;

Dot::Dot() : posX_(0), posY_(0), velX_(0), velY_(0) {
	for (auto &particle : particles_) {
		particle = new Particle(posX_, posY_);
	}
}

Dot::~Dot() {
	//Delete particles
	for (auto &particle : particles_) {
		delete particle;
	}
}

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
	if ((posX_ < 0) || (posX_ + DOT_WIDTH > LEVEL_WIDTH)) {
		//Move back
		posX_ -= velX_;
	}

	//Move dot up or down
	posY_ += velY_;

	//If dot went too far up or down
	if ((posY_ < 0) || (posY_ + DOT_WIDTH > LEVEL_HEIGHT)) {
		//Move back
		posY_ -= velY_;
	}
}

void Dot::render(int camX, int camY) {
	gDotTexture.render(posX_ - camX, posY_ - camY);
	renderParticles(camX, camY);
}

int Dot::getPosX() const {
	return posX_;
}

int Dot::getPosY() const {
	return posY_;
}

void Dot::renderParticles(int camX, int camY) {
	for (auto &particle : particles_) {
		if (particle->isDead()) {
			delete particle;
			particle = new Particle(posX_, posY_);
		}
	}

	for (auto &particle : particles_) {
		particle->render(camX, camY);
	}
}

