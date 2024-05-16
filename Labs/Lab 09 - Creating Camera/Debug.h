#ifndef LEARNOPENGL_DEBUG_H
#define LEARNOPENGL_DEBUG_H

class Debug
{
public:
	// Logs with text on the console
	static inline void Log(const char* text, bool bPause = false) { GetInstance().ILog(text, bPause); }

private:
	// Class's contructor and static reference
	Debug() {};
	static inline Debug& GetInstance() { static Debug m_Instance; return m_Instance; }

	// Delete copy constructor and assigment operator
	Debug(const Debug&) = delete;
	Debug operator=(const Debug&) = delete;

	// Logs with text on the console
	static inline void ILog(const char* text, bool bPause = false) { std::printf(text); if (bPause) { char Pause = std::getchar(); } }
};

#endif // !LEARNOPENGL_DEBUG_H
