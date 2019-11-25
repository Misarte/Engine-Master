#include "msTimer.h"
#include "Application.h"


msTimer::msTimer()
{
}

msTimer::~msTimer()
{
}

bool msTimer::Start()
{
	//currentTime = 0;
	return true;
}

void msTimer::Read()
{
	/*currentTime = SDL_GetTicks();
	if (currentTime > lastTime)
	{
		Stop();
	}*/
}

void msTimer::Stop()
{
	printf("Time: %d", lastTime);
	lastTime = currentTime;
}