#ifndef _DOT_H
#define _DOT_H

#include <SDL_events.h>
#include <SDL_rect.h>

#include "Particle.h"

class Dot {
public:
	//Dot dimensions
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;
	//Maximum dot axis velocity
	static const int DOT_VEL = 10;

	Dot();
	~Dot();
	//Take key presses and adjust dot velocity
	void handleEvent(SDL_Event&);
	//Move dot
	void move();
	//Show dot on screen
	void render(int camX, int camY);
	int getPosX() const;
	int getPosY() const;
private:
	//Particles
	Particle* particles_[TOTAL_PARTICLES];
	//Show particles
	void renderParticles(int camX, int camY);
	//X and Y offsets of dot
	int posX_, posY_;
	//Velocity of dot
	int velX_, velY_;
};

#endif
