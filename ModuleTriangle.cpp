#include "ModuleTriangle.h"
#include "MathGeoLib.h"
#include "ModuleProgram.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleCamera.h"
ModuleTriangle::ModuleTriangle()
{
}

// Destructor
ModuleTriangle::~ModuleTriangle()
{
}



bool ModuleTriangle::Init() {
	
	
	float vertices[] = {
		//position				//texture
		-0.5F, -0.5F, 0.0F,		0.0F, 0.0F,
		0.5F, -0.5F, 0.0F,		1.0F, -1.0F,
		0.5F, 0.5F, 0.0F,		1.0F, 1.0F,
		-0.5F, 0.5F, 0.0F,		-1.0F, 1.0F
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(4 * sizeof(float)));

	glBindVertexArray(0);

	App->program->LoadShader("default.vs", "default.fs");
	App->texture->LoadTexture("Lenna.png");
	return true;
}

update_status ModuleTriangle::PreUpdate()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return UPDATE_CONTINUE;
}

update_status ModuleTriangle::Update()
{
	glUseProgram(App->program->ProgramID);
	glUniformMatrix4fv(glGetUniformLocation(App->program->ProgramID,"model"), 1, GL_TRUE, &(App->camera->model[0][0]));
	glUniformMatrix4fv(glGetUniformLocation(App->program->ProgramID,"view"), 1, GL_TRUE, &(App->camera->view[0][0]));
	glUniformMatrix4fv(glGetUniformLocation(App->program->ProgramID, "proj"), 1, GL_TRUE, &(App->camera->proj[0][0]));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, App->texture->textureID);
	glUniform1i(glGetUniformLocation(App->program->ProgramID, "texture0"), 0);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	return UPDATE_CONTINUE;
}

update_status ModuleTriangle::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleTriangle::CleanUp()
{
	LOG("Destroying renderer");
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	//Destroy window

	return true;
}