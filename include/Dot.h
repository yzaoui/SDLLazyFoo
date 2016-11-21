#ifndef _DOT_H
#define _DOT_H

#include <SDL_events.h>
#include <SDL_rect.h>

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
	void move(SDL_Rect& wall);
	//Show dot on screen
	void render();
private:
	//X and Y offsets of dot
	int posX_, posY_;
	//Velocity of dot
	int velX_, velY_;
	//Collision box
	SDL_Rect collider_;
};

#endif
