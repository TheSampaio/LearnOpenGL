#include "Timer.h"

Timer::Timer(float velocity) // Initialize all variables
{
	time = 0.0f;
	elapsedTime = 0.0f;
	currentTime = 0.0f;
	Timer::velocity = velocity;
}

void Timer::Start() // Starts timer
{
	elapsedTime = glfwGetTime(); // Store the elapsed time to program a simple timer
}

void Timer::Update() // Increases timer
{
	currentTime = glfwGetTime(); // Store the application's current time

	if (currentTime - elapsedTime >= 1 / 60) // Simple timer
	{
		time += velocity;          // Increse rotation's value
		elapsedTime = currentTime; // Update timer
	}
}

double Timer::GetTime() // Returns time
{
	return time;
}