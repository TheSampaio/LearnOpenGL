#ifndef LEARNOPENGL_DEBUG_H
#define LEARNOPENGL_DEBUG_H

class Debug
{
public:
	// Main methods
	static inline void Log(const char* Text, bool bPause = false) { GetInstance().ILog(Text, bPause); }

private:
	// Class's contructor and static reference
	Debug() {};
	static inline Debug& GetInstance() { static Debug m_Instance; return m_Instance; }

	// Delete copy constructor and assigment operator
	Debug(const Debug&) = delete;
	Debug operator=(const Debug&) = delete;

	// Internal methods
	static inline void ILog(const char* Text, bool bPause = false) { std::printf(Text); if (bPause) { char Pause = std::getchar(); } }
};

#endif // !LEARNOPENGL_DEBUG_H
