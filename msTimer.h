#include "Module.h"
#include "SDL.h"
#include "GL/glew.h"

class msTimer
{
public:
	msTimer();
	~msTimer();

	bool Start();

	void Stop();
	void Read();
	
	unsigned int lastTime = 0, currentTime;

};

