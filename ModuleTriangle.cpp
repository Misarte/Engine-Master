#include "ModuleTriangle.h"
#include "MathGeoLib.h"
#include "ModuleProgram.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleTexture.h"
ModuleTriangle::ModuleTriangle()
{
}

// Destructor
ModuleTriangle::~ModuleTriangle()
{
}

float4x4 ModuleTriangle::LookAt(float3 eye, float3 target, float3 up) {
	math::float3 f(target - eye); 
	f.Normalize();
	math::float3 s(f.Cross(up)); 
	s.Normalize();
	math::float3 u(s.Cross(f));
	float4x4 matrix;
	matrix[0][0] = s.x; 
	matrix[0][1] = s.y; 
	matrix[0][2] = s.z;
	matrix[1][0] = u.x; 
	matrix[1][1] = u.y; 
	matrix[1][2] = u.z;
	matrix[2][0] = -f.x; 
	matrix[2][1] = -f.y; 
	matrix[2][2] = -f.z;
	matrix[0][3] = -s.Dot(eye); 
	matrix[1][3] = -u.Dot(eye); 
	matrix[2][3] = f.Dot(eye);
	matrix[3][0] = 0.0f; 
	matrix[3][1] = 0.0f; 
	matrix[3][2] = 0.0f; 
	matrix[3][3] = 1.0f;

	return matrix;
}

bool ModuleTriangle::Init() {
	float aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3(0,10,0);
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect);
	proj = frustum.ProjectionMatrix();
	model = float4x4::FromTRS(float3(0.0f, 0.0f, -4.0f),
			float3x3::RotateY(math::pi/4.0f), 
			float3(1.0f,1.0f, 1.0f));

	view = LookAt(math::float3(0.0f, 1.0f, 4.0f),
					math::float3(0.0f, 0.0f, 0.0f),
					math::float3(0.0f, 1.0f, 0.0f));

	float4x4 transform = proj * view * float4x4(model);
	
	float4 v1 = transform * float4(-1.0f, -1.0f, 0.0f, 1.0f);
	float4 v2 = transform * float4(1.0f, -1.0f, 0.0f, 1.0f);
	float4 v3 = transform * float4(0.0f, 1.0f, 0.0f, 1.0f);

	v1 = v1 / v1.w;
	v2 = v2 / v2.w;
	v3 = v3 / v3.w;
	

	/*float buffer_data[] = {
		v1.x, v1.y, v1.z,
		v2.x, v2.y, v2.z,
		v3.x, v3.y, v3.z };*/
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
	glUniformMatrix4fv(glGetUniformLocation(App->program->ProgramID,"model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->ProgramID,"view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->ProgramID, "proj"), 1, GL_TRUE, &proj[0][0]);
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

	//Destroy window

	return true;
}