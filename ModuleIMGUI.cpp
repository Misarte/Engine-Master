#include "ModuleIMGUI.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleModelLoader.h"
#include "ModuleCamera.h"
#include "ModuleTexture.h"
#include "ModuleInput.h"
#include "SDL.h"
#include "GL/glew.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "./IL/il.h"
#include "Globals.h"
#include "GL/glew.h"

#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::experimental::filesystem;

ModuleIMGUI::ModuleIMGUI()
{
}

ModuleIMGUI::~ModuleIMGUI()
{
}

bool ModuleIMGUI::Init()
{
	//glewInit();
	const char* glsl_version = "#version 130";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	//// Setup Platform/Renderer bindings
	//App->window->gl_context = SDL_GL_CreateContext(App->window->window);
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	//// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//show_demo_window = false;

	return true;
}

update_status ModuleIMGUI::PreUpdate()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	return UPDATE_CONTINUE;
}

update_status ModuleIMGUI::Update()
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("Main Menu")) 
	{
		ImGui::MenuItem("Demo Window", NULL, &show_demo_window);
		ImGui::MenuItem("About Window", NULL, &about_window);
		ImGui::MenuItem("Visit Githubpage", NULL, &git);
		ImGui::Separator();
		ImGui::MenuItem("Quit", "Alt+F4", &quit);
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Tools"))
	{
		//ImGui::MenuItem("Camera constrols Window", NULL, &show_another_window);
		ImGui::MenuItem("Console Window", NULL, &console_window);
		ImGui::MenuItem("Properties", NULL, &properties_window);
		ImGui::MenuItem("Configurations Window", NULL, &config_window);
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
		
	if (quit) 
	{
		return UPDATE_STOP;
	}
	if (git)
	{
		ShellExecute(0, 0, "https://github.com/Misarte/Engine-Master", 0, 0, SW_SHOW);
		git = false;
		//return UPDATE_STOP;

	}
	if (show_demo_window)
	{
		ImGui::ShowDemoWindow(&show_demo_window);
	}

//Show Camera controls window.
	if (show_another_window)
	{
		ImGui::Begin("Camera controls", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Checkbox("Grid Ground", &grid);
		ImGui::Checkbox("Show Axis", &axis);
		if (ImGui::TreeNode("Camera Position"))
		{
			ImGui::SliderFloat("X", &(App->camera->frustum.pos.x), -50.0f, 50.0f);
			ImGui::SliderFloat("Y", &(App->camera->frustum.pos.y), -50.0f, 50.0f);
			ImGui::SliderFloat("Z", &(App->camera->frustum.pos.z), -50.0f, 50.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Camera Up-Front Vectors"))
		{
			ImGui::SliderFloat("Up-X", &(App->camera->frustum.up.x), -10.0f, 10.0f);
			ImGui::SliderFloat("Up-Y", &(App->camera->frustum.up.y), -10.0f, 10.0f);
			ImGui::SliderFloat("Up-Z", &(App->camera->frustum.up.z), -10.0f, 10.0f);

			ImGui::SliderFloat("Front-X", &(App->camera->frustum.front.x), -10.0f, 10.0f);
			ImGui::SliderFloat("Front-Y", &(App->camera->frustum.front.y), -10.0f, 10.0f);
			ImGui::SliderFloat("Front-Z", &(App->camera->frustum.front.z), -10.0f, 10.0f);
			ImGui::TreePop();
		}
		ImGui::Text("Camera near distance: %.3f", App->camera->frustum.nearPlaneDistance);
		ImGui::Text("Camera far distance: %.3f", App->camera->frustum.farPlaneDistance);
		float fov = App->camera->frustum.verticalFov;
		if (ImGui::SliderFloat("Vertical FOV", &fov, 0.01f, math::pi, "%.3f", 1.0f))
		{
			App->camera->SetFOV(fov);
		}
		
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	if (grid)
	{
		App->camera->ShowGrid();
	}
	if (axis)
	{
		App->camera->ShowAxis();
	}
	if (properties_window)
	{

		ImGui::Begin("Properities", &properties_window);
		if (ImGui::TreeNode("Transformation"))
		{
			ImGui::Text("Position");
			ImGui::Text("X: %d", App->model->modelPos.x);
			ImGui::Text("Y: %d", App->model->modelPos.y);
			ImGui::Text("Z: %d", App->model->modelPos.z);
			//ImGui::Value("X", App->model->modelPos);
			/*ImGui::Text("Rotation");
			ImGui::Text("X: %d", App->model->boundingBox.);
			ImGui::SameLine();
			ImGui::Text("Y: %d", App->model->modelPos.y);
			ImGui::SameLine();*/
			ImGui::Text("Scale X: %f", App->model->boundingBox.Size().x);
			ImGui::Text("Scale Y: %f", App->model->boundingBox.Size().y);
			ImGui::Text("Scale Z: %f", App->model->boundingBox.Size().z);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Geometry"))
		{
			ImGui::Text("Meshes Loaded: %d", App->model->meshes.size());
			ImGui::Text("LoadedModel contains:");
			ImGui::Text("Vertices: %d", App->model->numVertices);
			ImGui::Text("Indices: %d", App->model->numIndices);
			ImGui::Text("Faces/Triangles: %d", App->model->numFaces);
			ImGui::Text("Center: %d", App->model->centerPoint);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Texture"))
		{
			ImGui::Text("Current Texture Width: %d", App->texture->width);
			ImGui::Text("Current Texture Height: %d", App->texture->height);
			for (unsigned int i=0; i<App->texture->textures_loaded.size(); i++)
			{
				ImGui::Text("Width: %d", App->texture->textures_loaded[i].width);
				ImGui::Text("Height: %d", App->texture->textures_loaded[i].height);
				if (ImGui::ImageButton((void*)(intptr_t)App->texture->textures_loaded[i].id, ImVec2(128,128)))
				{
					App->model->UpdateTexture(App->texture->textures_loaded[i]);
				}

			}
			ImGui::TreePop();
		}
		
		if (ImGui::Button("Close Me"))
			properties_window = false;
		ImGui::End();
	}
//	//show console window
	if (console_window)
	{
		ImGui::Begin("Console window", &console_window);
		ImGui::TextUnformatted(Buf.begin());
		if (ScrollToBottom)
			ImGui::SetScrollHere(1.0f);
		ScrollToBottom = false;
		ImGui::End();
		
	}
	if (window_info)
	{
		ImGui::Begin("Our window info", &window_info);
		ImGui::Text("Window width: %d", SDL_GetWindowSurface(App->window->window)->w);
		ImGui::Text("Window width: %d", SDL_GetWindowSurface(App->window->window)->h);
		ImGui::Checkbox("Set Full Screen", &full_screen);
		if (full_screen)
		{
			SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		}
		else
		{
			SDL_SetWindowFullscreen(App->window->window, 0);
		}
		if (ImGui::Button("Close Me"))
			window_info = false;
		ImGui::End();
	}
	if (renderer_window)
	{
		ImGui::Begin("Renderer info", &renderer_window);
		ImGui::Text("Using Glew %s\n", glewGetString(GLEW_VERSION));
		ImGui::Text("Vendor: %s\n", glGetString(GL_VENDOR));
		ImGui::Text("Renderer: %s\n", glGetString(GL_RENDERER));
		ImGui::Text("OpenGL version supported %s\n", glGetString(GL_VERSION));
		ImGui::Text("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
		if (ImGui::Button("Close Me"))
			renderer_window = false;
		ImGui::End();
	}
	if (input_window)
	{
		ImGui::Begin("Inputs info (Only Valid and Combos)", &input_window);
		ImGui::TextUnformatted(BufInput.begin());
		if (ScrollToBottom2)
			ImGui::SetScrollHere(1.0f);
		ScrollToBottom2 = false;
		if (ImGui::Button("Close Me"))
			input_window = false;
		ImGui::End();
	}
	
	// Show config window
	if (config_window)
	{
		ImGui::Begin("Configuration window", &config_window);
		ImGui::Separator();
		if (ImGui::TreeNode("Modules"))
		{
			ImGui::Checkbox("Window", &window_info);
			ImGui::Checkbox("Camera", &show_another_window);
			//ImGui::Checkbox("IMGUI", &imgui_window);
			ImGui::Checkbox("Input", &input_window);
			ImGui::Checkbox("Renderer", &renderer_window);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Libraries Configuration"))
		{
			ImGui::BeginChildFrame(ImGui::GetID("libraries"), ImVec2(ImGui::GetWindowContentRegionWidth() - 45, ImGui::GetTextLineHeightWithSpacing() * 7), ImGuiWindowFlags_NoMove);
			//ImGui::Text("IMGUI: %s", ImGui::GetVersion());
			ImGui::Text("Dear ImGui %s (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);
			ImGui::Separator();
			SDL_version compiled;
			SDL_version linked;
			SDL_VERSION(&compiled);
			SDL_GetVersion(&linked);

			ImGui::Text("SDL Compiled version: %d.%d.%d\n", compiled.major, compiled.minor, compiled.patch);
			ImGui::Text("SDL Linked version: %d.%d.%d.\n", linked.major, linked.minor, linked.patch);
			ImGui::Separator();
			const char* versionGL = (char *)(glGetString(GL_VERSION));
			ImGui::Text("Glew Version: %s", versionGL);
			ImGui::Separator();
			ImGui::Text("MathGeo Library");
			ImGui::Separator();
			ImGui::Text("DevIl Library");
			ImGui::Separator();
			ImGui::Text("Assimp Library");
			ImGui::EndChildFrame();
			ImGui::TreePop();
		}
		if (ImGui::Button("Close Me"))
			config_window = false;
		ImGui::End();
	}
	// Show about  window.
	if (about_window)
	{
		ImGui::Begin("About Window", &about_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

		//from demo
		ImGui::Text("Artemis Engine v1");
		ImGui::Separator();
		ImGui::Text("By Artemis Georgakopoulou.");
		ImGui::Text("Artemis Engine is licensed under the MIT License, see LICENSE for more information.");

		static bool show_config_info = false;
		ImGui::Checkbox("Config/Build Information", &show_config_info);
		if (show_config_info)
		{
			//ImGuiIO& io = ImGui::GetIO();
			ImGuiStyle& style = ImGui::GetStyle();
			if (ImGui::TreeNode("System Configuration"))
			{

				bool copy_to_clipboard = ImGui::Button("Copy to clipboard");
				ImGui::BeginChildFrame(ImGui::GetID("cfginfos"), ImVec2(ImGui::GetWindowContentRegionWidth() - 45, ImGui::GetTextLineHeightWithSpacing() * 7), ImGuiWindowFlags_NoMove);
				if (copy_to_clipboard)
				{
					ImGui::LogToClipboard();
					ImGui::LogText("```\n"); // Back quotes will make the text appears without formatting when pasting to GitHub
				}
				int cpus = SDL_GetCPUCount();
				ImGui::Text("CPUs: %d logical cores", cpus);
				int ram = SDL_GetSystemRAM();
				ImGui::Text("RAM: %d MB", ram);
				ImGui::Text("GPU Vendor: %s", glGetString(GL_VENDOR));
				ImGui::Text("GPU Model: %s", glGetString(GL_RENDERER));
				const char* platform = SDL_GetPlatform();
				ImGui::Text("Platform: %s ", platform);

				//ImGui::Text("sizeof(size_t): %d, sizeof(ImDrawIdx): %d, sizeof(ImDrawVert): %d", (int)sizeof(size_t), (int)sizeof(ImDrawIdx), (int)sizeof(ImDrawVert));

				ImGui::Text("define: __cplusplus=%d", (int)__cplusplus);
#ifdef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
				ImGui::Text("define: IMGUI_DISABLE_OBSOLETE_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS
				ImGui::Text("define: IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS
				ImGui::Text("define: IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_WIN32_FUNCTIONS
				ImGui::Text("define: IMGUI_DISABLE_WIN32_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_FORMAT_STRING_FUNCTIONS
				ImGui::Text("define: IMGUI_DISABLE_FORMAT_STRING_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_MATH_FUNCTIONS
				ImGui::Text("define: IMGUI_DISABLE_MATH_FUNCTIONS");
#endif
#ifdef IMGUI_DISABLE_DEFAULT_ALLOCATORS
				ImGui::Text("define: IMGUI_DISABLE_DEFAULT_ALLOCATORS");
#endif
#ifdef IMGUI_USE_BGRA_PACKED_COLOR
				ImGui::Text("define: IMGUI_USE_BGRA_PACKED_COLOR");
#endif
#ifdef _WIN32
				ImGui::Text("define: _WIN32");
#endif
#ifdef _WIN64
				ImGui::Text("define: _WIN64");
#endif
#ifdef __linux__
				ImGui::Text("define: __linux__");
#endif
#ifdef __APPLE__
				ImGui::Text("define: __APPLE__");
#endif
#ifdef _MSC_VER
				ImGui::Text("define: _MSC_VER=%d", _MSC_VER);
#endif
#ifdef __MINGW32__
				ImGui::Text("define: __MINGW32__");
#endif
#ifdef __MINGW64__
				ImGui::Text("define: __MINGW64__");
#endif
#ifdef __GNUC__
				ImGui::Text("define: __GNUC__=%d", (int)__GNUC__);
#endif
#ifdef __clang_version__
				ImGui::Text("define: __clang_version__=%s", __clang_version__);
#endif

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::Separator();
				

				if (copy_to_clipboard)
				{
					ImGui::LogText("\n```\n");
					ImGui::LogFinish();
				}
				ImGui::EndChildFrame();

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Engine Configuration"))
			{
				ImGui::BeginChildFrame(ImGui::GetID("engine"), ImVec2(ImGui::GetWindowContentRegionWidth() - 45, ImGui::GetTextLineHeightWithSpacing() * 5), ImGuiWindowFlags_NoMove);
				ImGui::Text("Name: Artemis Game Engine");
				//ImGui::Image();
				ImGui::Separator();
				ImGui::Text("Description: Artemis Game Engine");
				ImGui::Separator();
				ImGui::Text("Author: Artemis Georgakopoulou");
				ImGui::EndChildFrame();
				ImGui::TreePop();
			}
			fps_log.push_back(ImGui::GetIO().Framerate);
			if (fps_log.size() > 110) {
				char title[25];
				sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
				ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(350, 100));
				fps_log.erase(fps_log.begin());
			}
		}
		if (ImGui::Button("Close Me"))
			about_window = false;
		ImGui::End();
	}
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

update_status ModuleIMGUI::PostUpdate()
{
	return UPDATE_CONTINUE;
}

void  ModuleIMGUI::AddLog(const char* fmt, ...)
{
	static va_list  ap;
	va_start(ap, fmt);
	Buf.appendfv(fmt, ap);
	va_end(ap);
	ScrollToBottom = true;
}
void  ModuleIMGUI::AddLogInput(const char* fmt, ...)
{
	static va_list  another;
	va_start(another, fmt);
	BufInput.appendfv(fmt, another);
	va_end(another);
	ScrollToBottom2 = true;
}
bool ModuleIMGUI::CleanUp()
{
	LOG("Destroying program");
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_Quit();
	//Destroy window
	return true;
}