#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleTriangle;
class ModuleProgram;
class ModuleRenderExercise;
class ModuleIMGUI;
class ModuleTexture;
class ModuleCamera;
class ModuleModelLoader;
class msTimer;


class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	
	ModuleRender * renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleTriangle* triangle = nullptr;
	ModuleProgram* program = nullptr;
	ModuleIMGUI* imgui = nullptr;
	ModuleTexture* texture = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleModelLoader* model = nullptr;
private:

	std::list<Module*> modules;

};

extern Application* App;
