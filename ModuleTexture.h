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

	GLuint LoadTexture(const char*);

	int width, height, nrChannels;
	ILuint imageName;
	GLuint texture_object;
	GLuint textureID;
	bool CleanUp();

};

