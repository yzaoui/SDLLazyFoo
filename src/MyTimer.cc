#include "MyTimer.h"

#include <SDL_timer.h>

MyTimer::MyTimer() : startedTicks_(0), pausedTicks_(0),
	started_(false), paused_(false) {}

void MyTimer::start() {
	started_ = true;
	paused_ = false;

	startedTicks_ = SDL_GetTicks();
	pausedTicks_ = 0;
}

void MyTimer::stop() {
	started_ = false;
	paused_ = false;

	startedTicks_ = 0;
	pausedTicks_ = 0;
}

void MyTimer::pause() {
	if (started_ && !paused_) {
		paused_ = true;

		pausedTicks_ = SDL_GetTicks() - startedTicks_;
		startedTicks_ = 0;
	}
}

void MyTimer::unpause() {
	if (started_ && paused_) {
		paused_ = false;

		startedTicks_ = SDL_GetTicks() - pausedTicks_;

		pausedTicks_ = 0;
	}
}

uint32_t MyTimer::getTicks() const {
	if (started_) {
		if (paused_) {
			//If started and paused, return paused time
			return pausedTicks_;
		}
		//If started and unpaused, return relative started time
		return (SDL_GetTicks() - startedTicks_);
	}

	//If not started yet, return 0
	return 0;
}

bool MyTimer::isStarted() const {
	return started_;
}

bool MyTimer::isPaused() const {
	return paused_;
}

