
#include "Module.h"
#include "Globals.h"
#include "SDL.h"
#include <vector>
#include <iostream>
#include "imgui.h"

class ModuleIMGUI :
	public Module
{
public:
	ModuleIMGUI();
	~ModuleIMGUI();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	//SDL_GLContext gl_context = NULL;
	bool show_demo_window = false;
	bool show_another_window = false;
	bool console_window = false;
	bool about_window = false;
	bool grid = false;
	bool axis = false;

	//vector <float> logs;
	void AddLog();
	bool CleanUp();
};
