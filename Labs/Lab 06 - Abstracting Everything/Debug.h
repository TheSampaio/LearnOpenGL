#ifndef LEARNOPENGL_DEBUG_H
#define LEARNOPENGL_DEBUG_H

class Debug
{
public:
	// Logs with text on the console
	static inline void Log(const char* Text, bool bPause = false) { GetInstance().ILog(Text, bPause); }

private:
	// Class's contructor and static reference
	Debug() {};
	static inline Debug& GetInstance() { static Debug s_Instance; return s_Instance; }

	// Delete copy constructor and assigment operator
	Debug(const Debug&) = delete;
	Debug operator=(const Debug&) = delete;

	// Logs with text on the console
	static inline void ILog(const char* Text, bool bPause = false) { std::printf(Text); if (bPause) { char pause = std::getchar(); } }
};

#endif // !LEARNOPENGL_DEBUG_H
