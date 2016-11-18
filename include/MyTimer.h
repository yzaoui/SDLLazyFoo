#ifndef _MYTIMER_H
#define _MYTIMER_H

#include <cstdint>

class MyTimer {
public:
	//Initializes variables
	MyTimer();

	//Various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Get timer's time
	uint32_t getTicks() const;

	//Check status of the timer
	bool isStarted() const;
	bool isPaused() const;
private:
	//Clock time when the timer started
	uint32_t startedTicks_;

	//Ticks stored when the timer was paused
	uint32_t pausedTicks_;

	//Timer status
	bool started_;
	bool paused_;
};

#endif
