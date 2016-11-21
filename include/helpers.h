#ifndef _HELPERS_H
#define _HELPERS_H

/**
 * Detects box collisions.
 *
 * @param  a The first object's collision box.
 * @param  b The second object's collision box.
 *
 * @return   true if a and b are colliding, false otherwise.
 */
bool checkCollision(SDL_Rect a, SDL_Rect b) {
	if ((a.y + a.h <= b.y)
		|| (a.y >= b.y + b.h)
		|| (a.x + a.w <= b.x)
		|| (a.x >= b.x + b.w)) {
		return false;
	}

	//If none of the sides of A are outside of B
	return true;
}

#endif
