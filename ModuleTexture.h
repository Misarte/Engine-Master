#pragma once
#include "Module.h"
#include "SDL.h"
#include "GL/glew.h"
#include "MathGeoLib.h"
#include "IL/il.h"
class ModuleTexture :
	public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	ILuint LoadTexture(const char*);

	int width, height, nrChannels;
	ILubyte *data = nullptr;
	GLuint texture_object;
	GLuint textureID;
	bool CleanUp();

};

