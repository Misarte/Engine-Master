#include "ModuleIMGUI.h"
#include "ModuleCamera.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "SDL.h"
#include "GL/glew.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"
#include "./IL/il.h"
#include "Globals.h"

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
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Camera constrols Window", &show_another_window);
		ImGui::Checkbox("Console Window", &console_window);
		ImGui::Checkbox("About Window", &about_window);
		//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
		
		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Camera controls", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Checkbox("Grid Ground", &grid);
		ImGui::Checkbox("Show Axis", &axis);
		if (ImGui::TreeNode("Camera Position"))
		{
			ImGui::SliderFloat("X", &(App->camera->frustum.pos.x), -10.0f, 10.0f);
			ImGui::SliderFloat("Y", &(App->camera->frustum.pos.y), -10.0f, 10.0f);
			ImGui::SliderFloat("Z", &(App->camera->frustum.pos.z), -10.0f, 10.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Camera Rotation"))
		{
			ImGui::SliderFloat("Up-X", &(App->camera->frustum.up.x), -10.0f, 10.0f);
			ImGui::SliderFloat("Up-Y", &(App->camera->frustum.up.y), -10.0f, 10.0f);
			ImGui::SliderFloat("Up-Z", &(App->camera->frustum.up.z), -10.0f, 10.0f);
			ImGui::SliderFloat("Front-X", &(App->camera->frustum.front.x), -10.0f, 10.0f);
			ImGui::SliderFloat("Front-Y", &(App->camera->frustum.front.y), -10.0f, 10.0f);
			ImGui::SliderFloat("Front-Z", &(App->camera->frustum.front.z), -10.0f, 10.0f);
			ImGui::TreePop();
		}
		
		ImGui::Text("Hello from another window!");
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

	//show console window
	if (console_window)
	{
		//ImGuiTextBuffer     Buf;
		//ImGuiTextFilter     Filter;
		//ImVector<int>       LineOffsets;        // Index to lines offset
		//bool                ScrollToBottom;

		////LOG(Buf);
		//ScrollToBottom = true;
		////Buf  = SDL_LOG
		//ImGui::Begin("Console window", &console_window);
		//ImGui::TextUnformatted(Buf.begin());
		//if (ScrollToBottom)
		//	ImGui::SetScrollHere(1.0f);
		//ScrollToBottom = false;
		//if (ImGui::Button("Close Me"))
		//	console_window = false;
		//ImGui::End();


	}
	// 3. Show about simple window.
	if (about_window)
	{
		ImGui::Begin("About Window", &about_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

		//from demo
		ImGui::Text("Artemis Enginev v1");
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
				const char* platform = SDL_GetPlatform();
				ImGui::Text("Platform: %s ", platform);

				ImGui::Text("sizeof(size_t): %d, sizeof(ImDrawIdx): %d, sizeof(ImDrawVert): %d", (int)sizeof(size_t), (int)sizeof(ImDrawIdx), (int)sizeof(ImDrawVert));

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
				/*ImGui::Separator();
				const char* devil = (char *)(glGetString(GL_VERSION));
				ImGui::Text("DevIL: %s", devil);*/
				ImGui::Separator();
				ImGui::Text("MathGeo Library");
				/*std::string path = "/MathGeoLib/include/";
				for (const auto & entry : fs::directory_iterator(path))
				ImGui::Text("MathGeo File: %s");*/

				ImGui::Separator();
				//ILuint ilversion = iluGetInteger(ILU_VERSION_NUM);
				ImGui::Text("DevIl Library");
				ImGui::EndChildFrame();
				ImGui::TreePop();
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