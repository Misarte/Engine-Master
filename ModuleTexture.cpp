#include "ModuleTexture.h"
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
	loaded = ilLoadImage(texture_path);
	//iluFlipImage();
	ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	texture.width = ilGetInteger(IL_IMAGE_WIDTH);
	texture.height = ilGetInteger(IL_IMAGE_HEIGHT);
	texture.data = ilGetData();
	texture.id = ilutGLBindTexImage();
	texture.path = texture_path;
	return texture;
}

bool ModuleTexture::CleanUp()
{
	ilDeleteImages(1, &imageName);
	return true;
}
