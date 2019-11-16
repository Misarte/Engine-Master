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
	bool ok = ilLoadImage(texture_path);
	//iluGetImageInfo(&ImageInfo);
	iluFlipImage();
	ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	texture.width = ilGetInteger(IL_IMAGE_WIDTH);
	texture.height = ilGetInteger(IL_IMAGE_HEIGHT);
	texture.data = ilGetData();
	texture.id = ilutGLBindTexImage();
	texture.path = texture_path;
	App->imgui->AddLog("PATH---------------%s\n", texture.path);
	/*glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture.data);
	glGenerateMipmap(GL_TEXTURE_2D);*/
	return texture;
}

bool ModuleTexture::CleanUp()
{
	ilDeleteImages(1, &imageName);
	return true;
}
