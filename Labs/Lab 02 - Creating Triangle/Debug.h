#ifndef LEARNOPENGL_DEBUG_H
#define LEARNOPENGL_DEBUG_H

#include <iostream>

class Debug
{
public:
	// Main methods
	static inline void Log(const char* Text, bool bPause = false) { Get().ILog(Text, bPause); }

private:
	// Class's contructor and static reference
	Debug() {};
	static inline Debug& Get() { static Debug s_Instance; return s_Instance; }

	// Delete copy constructor and assigment operator
	Debug(const Debug&) = delete;
	Debug operator=(const Debug&) = delete;

	// Internal methods
	static inline void ILog(const char* Text, bool bPause = false) { std::printf(Text); if (bPause) { char pause = std::getchar(); } }
};

#endif // !LEARNOPENGL_DEBUG_H
