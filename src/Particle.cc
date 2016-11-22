#include "Particle.h"

extern MyTexture gRedTexture;
extern MyTexture gGreenTexture;
extern MyTexture gBlueTexture;
extern MyTexture gShimmerTexture;

Particle::Particle(int x, int y) {
	//Set offsets
	posX_ = x - 5 + (rand() % 25);
	posY_ = y - 5 + (rand() % 25);

	//Initialize animation
	frame_ = rand() % 5;

	//Set type
	switch (rand() % 3) {
		case 0: texture_ = &gRedTexture; break;
		case 1: texture_ = &gGreenTexture; break;
		case 2: texture_ = &gBlueTexture; break;
	}
}

void Particle::render(int camX, int camY) {
	//Show image
	texture_->render(posX_ - camX, posY_ - camY);

	//Show shimmer
	if (frame_ % 2 == 0) {
		gShimmerTexture.render(posX_ - camX, posY_ - camY);
	}

	//Animate
	frame_++;
}

bool Particle::isDead() {
	return frame_ > 10;
}
