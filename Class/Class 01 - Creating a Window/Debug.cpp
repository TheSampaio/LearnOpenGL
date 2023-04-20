#include "Debug.h"

void Debug::Log(const char* Text, bool bPause)
{
	(bPause) ? std::printf("%s\n", Text) : std::printf(Text);
}
