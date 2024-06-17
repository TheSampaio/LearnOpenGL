#ifndef LEARNOPENGL_TIMER_H
#define LEARNOPENGL_TIMER_H

class Timer
{
public:
	// Returns a reference for the timer
	static inline Timer& GetInstance() { static Timer s_Instance; return s_Instance; }

	// Updates the timer and updates the delta time
	void Update();

	// Returns the amount time
	inline float GetAmountTime() const { return m_Amount; }

	// Returns the delta time
	inline float GetDeltaTime() const { return m_Delta; }

private:
	Timer();

	// Deletes copy contructor and assigment operator
	Timer(const Timer&) = delete;
	Timer operator=(const Timer&) = delete;

	// Attributes
	float m_Amount, m_Current, m_Elapsed, m_Delta;
};

#endif // !LEARNOPENGL_TIMER_H
