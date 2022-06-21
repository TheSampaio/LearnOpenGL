#ifndef TIMER_CLASS_H
#define TIMER_CLASS_H

#include <GLFW/glfw3.h>

class Timer
{
public:
	Timer(float velocity);

	float velocity;
	double time;
	double elapsedTime;
	double currentTime;

	void Start();
	void Update();
	double GetTime();
};
#endif // !TIMER_CLASS_H