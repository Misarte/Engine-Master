#include "ModuleTexture.h"
#include "ModuleTriangle.h"
#include "MathGeoLib.h"
#include "ModuleProgram.h"
#include "Globals.h"
#include "Application.h"
#include "GL/glew.h"
#include "MathGeoLib.h"
#include "IL/il.h"


ModuleTexture::ModuleTexture()
{
}


ModuleTexture::~ModuleTexture()
{
}
GLuint ModuleTexture::LoadTexture(const char* texture_path)
{
	ilLoadImage(texture_path);
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	data = ilGetData();
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	/*ILinfo ImageInfo;
	iluGetImageInfo(&ImageInfo);
	if (ImageInfo.Origin == IL_OROGIN_UPPER_LEFT)
	{
		iluFlipImage();
	}*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	return textureID;
}

bool ModuleTexture::Init() 
{
	float buffer_data[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f};

	//ilDeleteImages(1, &textureID);
	return true;
}

update_status ModuleTexture::PreUpdate()
{

	return UPDATE_CONTINUE;
}
update_status ModuleTexture::Update()
{
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * 3));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_object);
	//glUniform1i(glGetUniformLocation(App->program->, "texture0"), 0);
	return UPDATE_CONTINUE;
}
update_status ModuleTexture::PostUpdate()
{

	return UPDATE_CONTINUE;
}

bool ModuleTexture::CleanUp()
{
	return true;
}
