#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	// Called before quitting
	bool Init();

	// Called before quitting
	bool CleanUp();

public:
	//The window we'll be rendering to
	SDL_Window* window = NULL;
	Uint32 flags;
	int width = SCREEN_WIDTH;
	int height = SCREEN_HEIGHT;
	int disScreenWidthMax = 0;
	int disScreenHeightMax = 0;
	int disScreenWidthMin = 0;
	int disScreenHeightMin = 0;
	float aspect = 2.0f;
	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;
	void Rescale(unsigned int newW, unsigned int newH);
};

#endif // __ModuleWindow_H__