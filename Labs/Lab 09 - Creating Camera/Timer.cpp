#include "PCH.h"
#include "Timer.h"

Timer::Timer()
{
	m_Amount = 0.0f;
	m_Current = 0.0f;
	m_Elapsed = 0.0f;
	m_Delta = 0.0f;
}

void Timer::Update()
{
	// Calculates delta time
	m_Current = static_cast<float>(glfwGetTime());
	m_Delta = m_Current - m_Elapsed;
	m_Amount += m_Delta;
	m_Elapsed = m_Current;
}
