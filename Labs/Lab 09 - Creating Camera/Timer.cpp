#include "PCH.h"
#include "Timer.h"

Timer::Timer()
{
	m_Start = 0.0f;
	m_End = 0.0f;
	m_DeltaTime = 0.0f;
}

void Timer::Reset()
{
	// Increase timer
	m_End = static_cast<float>(glfwGetTime());

	m_DeltaTime += (m_End - m_Start);
	m_Start = m_End;
}
