#ifndef __MODULEINPUT_H__
#define __MODULEINPUT_H__
#include "Module.h"
#include "Globals.h"
#include "SDL_scancode.h"
#include "./Point.h"
#include <string>

#define NUM_MOUSE_BUTTONS 3
enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};
typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();

	// Check key states (includes mouse and joy buttons)
	KeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	Uint32 GetMouseButtonDown(int id) const
	{
		return SDL_GetMouseState(NULL, NULL);
	}

	// Check for window events last frame
	bool GetWindowEvent(EventWindow code) const;
	// Get mouse / axis position
	const float2& GetMouseMotion() const;
	const float2& GetMousePosition() const;
	std::string dropped_filedir;
private:
	bool windowEvents[WE_COUNT];
	KeyState* keyboard = NULL;
	KeyState mouse_buttons[NUM_MOUSE_BUTTONS];
	float2 mouse_motion;
	float2 mouse;
	
//private:
//	const Uint8 *keyboard = NULL;
};
#endif // __MODULEINPUT_H__