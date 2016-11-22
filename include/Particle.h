#ifndef _PARTICLE_H
#define _PARTICLE_H

#include "MyTexture.h"

const int TOTAL_PARTICLES = 20;

class Particle {
public:
	Particle(int x, int y);
	void render();
	bool isDead();
private:
	int posX_, posY_;
	int frame_;
	MyTexture* texture_;
};

#endif
