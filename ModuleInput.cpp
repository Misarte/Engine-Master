#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleCamera.h"
#include "ModuleIMGUI.h"
#include "ModuleWindow.h"
#include "ModuleTriangle.h"
#include "ModuleTexture.h"
#include "ModuleProgram.h"
#include "ModuleModelLoader.h"
#include "SDL.h"
#include <iostream>
#include <string>

#define MAX_KEYS 300

ModuleInput::ModuleInput() : Module(), mouse({ 0, 0 }), mouse_motion({ 0,0 })
{
	keyboard = new KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	//RELEASE_ARRAY(keyboard);
}

// Called before render is available
bool ModuleInput::Init()
{
	App->imgui->AddLog("Init SDL input event system\n");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		App->imgui->AddLog("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called each loop iteration
update_status ModuleInput::PreUpdate()
{
	

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleInput::Update()
{
	SDL_PumpEvents();
	static SDL_Event event;

	mouse_motion = { 0, 0 };
	memset(windowEvents, false, WE_COUNT * sizeof(bool));

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	//SDL_Get

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if (mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}

	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[WE_HIDE] = true;
				break;

				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[WE_SHOW] = true;
				break;
			case SDL_WINDOWEVENT_RESIZED:
				App->window->Rescale(event.window.data1, event.window.data2);
				break;
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				App->window->Rescale(event.window.data1, event.window.data2);
				break;
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[event.button.button - 1] = KEY_DOWN;
			break;

		//case SDL_MOUSEMOTION:
		//	if (event.button.state && SDL_BUTTON_RIGHT)
		//	{
		//		if (math::Abs(event.motion.xrel)>1)
		//		{
		//			//App->camera->RotateCam("Y", event.motion.xrel * 0.03);
		//		}
		//	}
		//	break;

		case SDL_MOUSEBUTTONUP:
			mouse_buttons[event.button.button - 1] = KEY_UP;
			break;
		case SDL_MOUSEWHEEL:
			if (event.wheel.y > 0)
			{
				App->camera->ZoomIn();
			}
			else if (event.wheel.y < 0)
			{
				App->camera->ZoomOut();
			}
			/*mouse_buttons[event.button.button - 1] = KEY_DOWN;*/
			break;

		case SDL_MOUSEMOTION:
			mouse_motion.x = event.motion.xrel / SCREEN_SIZE;
			mouse_motion.y = event.motion.yrel / SCREEN_SIZE;
			mouse.x = event.motion.x / SCREEN_SIZE;
			mouse.y = event.motion.y / SCREEN_SIZE;
			break;
		case SDL_DROPFILE:
			App->imgui->AddLog("FILE DROPPED from:%s\n", event.drop.file);
			dropped_filedir = event.drop.file;
			if (dropped_filedir.substr(dropped_filedir.find_last_of(".") + 1) == "fbx")
			{
				App->imgui->AddLog("MODEL DROPPED from:%s\n", event.drop.file);
				App->model->LoadModel(dropped_filedir.c_str());
				for (unsigned int i = 0; i < App->model->meshes.size(); i++)
				{
					App->triangle->SetUpMesh(App->model->meshes[i]);
				}
			}
			else if (dropped_filedir.substr(dropped_filedir.find_last_of(".") + 1) == "png")
			{
				Mesh loadedMesh;
				//loadedMesh = App->model->GetLoadedMesh();
				//App->imgui->AddLog("TEXT LOADED BEFOR%d\n", App->model->);
				App->imgui->AddLog("TEXTURE DROPPED from:%s\n", event.drop.file);
				App->texture->LoadTexture(dropped_filedir.c_str());
				App->imgui->AddLog("TEXT LOADED AFTER%d\n", App->model->textures_loaded.size());
				
			}
			else
			{
				App->imgui->AddLog("Not Supported file drop");
			}
			SDL_free(event.drop.file);
			break;
		}

	}
	if (GetWindowEvent(EventWindow::WE_QUIT) == true || GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	App->imgui->AddLog("Quitting SDL input event subsystem\n");
	return true;
}
bool ModuleInput::GetWindowEvent(EventWindow ev) const
{
	return windowEvents[ev];
}

const iPoint& ModuleInput::GetMousePosition() const
{
	return mouse;
}

const iPoint& ModuleInput::GetMouseMotion() const
{
	return mouse_motion;
}



