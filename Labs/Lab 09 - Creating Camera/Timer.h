#ifndef LEARNOPENGL_TIMER_H
#define LEARNOPENGL_TIMER_H

class Timer
{
public:
	// Returns a reference for the timer
	static inline Timer& GetInstance() { static Timer m_Instance; return m_Instance; }

	// Resets the timer and updates the delta time
	void Reset();

	// Starts the timer
	inline void Start() { m_Start = static_cast<float>(glfwGetTime()); }

	// Returns the delta time
	inline float GetDeltaTime() { return m_DeltaTime; }

private:
	Timer();

	// Deletes copy contructor and assigment operator
	Timer(const Timer&) = delete;
	Timer operator=(const Timer&) = delete;

	// Attributes
	float m_Start, m_End, m_DeltaTime;
};

#endif // !LEARNOPENGL_TIMER_H
