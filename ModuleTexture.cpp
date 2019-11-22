#include "ModuleTexture.h"
#include "ModuleModelLoader.h"
#include "ModuleTriangle.h"
#include "ModuleIMGUI.h"
#include "MathGeoLib.h"
#include "ModuleProgram.h"
#include "Globals.h"
#include "Application.h"
#include "GL/glew.h"
#include "MathGeoLib.h"
#include "IL/il.h"
#include "IL/ilut.h"


ModuleTexture::ModuleTexture()
{
}


ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::Init()
{
	//ilDeleteImages(1, &textureID);
	ilInit();
	iluInit();
	ilutRenderer(ILUT_OPENGL);
	ilGenImages(1, &imageName);
	ilBindImage(imageName);
	return true;
}

Texture ModuleTexture::LoadTexture(const char* texture_path)
{
	App->model->textures_loaded.clear();
	loaded = ilLoadImage(texture_path);
	//iluFlipImage();
	ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	texture.width = ilGetInteger(IL_IMAGE_WIDTH);
	texture.height = ilGetInteger(IL_IMAGE_HEIGHT);
	texture.data = ilGetData();
	texture.id = ilutGLBindTexImage();
	texture.path = texture_path;
	App->imgui->AddLog("Adding Texture\n");
	width = texture.width;
	height = texture.height;
	return texture;
}

bool ModuleTexture::CleanUp()
{
	ilDeleteImages(1, &imageName);
	for (unsigned int i = 0; i < App->model->textures_loaded.size(); ++i)
	{
		glDeleteTextures(1, &(App->model->textures_loaded[i].id));

	}

	App->model->textures_loaded.clear();
	return true;
}
