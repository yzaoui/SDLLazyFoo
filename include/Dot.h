#ifndef _DOT_H
#define _DOT_H

#include <SDL.h>

class Dot {
public:
	//Dot dimensions
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;
	//Maximum dot axis velocity
	static const int DOT_VEL = 10;

	Dot();
	//Take key presses and adjust dot velocity
	void handleEvent(SDL_Event& e);
	//Move dot
	void move();
	//Show dot on screen
	void render();
private:
	//X and Y offsets of dot
	int posX_, posY_;
	//Velocity of dot
	int velX_, velY_;
};

#endif
