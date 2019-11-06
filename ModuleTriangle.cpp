#include "ModuleTriangle.h"
#include "MathGeoLib.h"
#include "ModuleProgram.h"
#include "Globals.h"
#include "Application.h"
ModuleTriangle::ModuleTriangle()
{
}

// Destructor
ModuleTriangle::~ModuleTriangle()
{
}

float4x4 ModuleTriangle::LookAt(float3 eye, float3 target, float3 up) {
	math::float3 f(target - eye); f.Normalize();
	math::float3 s(f.Cross(up)); s.Normalize();
	math::float3 u(s.Cross(f));
	float4x4 matrix;
	matrix[0][0] = s.x; matrix[0][1] = s.y; matrix[0][2] = s.z;
	matrix[1][0] = u.x; matrix[1][1] = u.y; matrix[1][2] = u.z;
	matrix[2][0] = -f.x; matrix[2][1] = -f.y; matrix[2][2] = -f.z;
	matrix[0][3] = -s.Dot(eye); matrix[1][3] = -u.Dot(eye); matrix[2][3] = f.Dot(eye);
	matrix[3][0] = 0.0f; matrix[3][1] = 0.0f; matrix[3][2] = 0.0f; matrix[3][3] = 1.0f;

	return matrix;
}

bool ModuleTriangle::Init() {
	Frustum frustum;
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * 2.0f);
	proj = frustum.ProjectionMatrix();
	model = 
				float4x4::FromTRS(float3(0.0f, 0.0f, -4.0f),
				float3x3::RotateY(math::pi/4.0f), 
				float3(1.0f,1.0f, 1.0f));

	view = LookAt(math::float3(0.0f, 1.f, 4.0f),
		math::float3(0.0f, 0.0f, 0.0f),
		math::float3(0.0f, 1.0f, 0.0f));

	float4x4 transform = proj * view * float4x4(model);
	
	float4 v1 = transform * float4(-1.0f, -1.0f, 0.0f, 1.0f);
	float4 v2 = transform * float4(1.0f, -1.0f, 0.0f, 1.0f);
	float4 v3 = transform * float4(0.0f, 1.0f, 0.0f, 1.0f);

	v1 = v1 / v1.w;
	v2 = v2 / v2.w;
	v3 = v3 / v3.w;
	

	float buffer_data[] = {
		v1.x, v1.y, v1.z,
		v2.x, v2.y, v2.z,
		v3.x, v3.y, v3.z };

	
	//math::float3(buffer_data);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_data), buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	return true;
}

update_status ModuleTriangle::PreUpdate()
{
	App->program->LoadShader("default.vs", "default.fs");
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return UPDATE_CONTINUE;
}

update_status ModuleTriangle::Update()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	int vertexLocation = glGetUniformLocation(App->program->ProgramID, "gl_Position");

	glUseProgram(App->program->ProgramID);
	glUniformMatrix4fv(glGetUniformLocation(App->program->ProgramID,"model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->ProgramID,"view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->program->ProgramID,"proj"), 1, GL_TRUE, &proj[0][0]);
	glEnableVertexAttribArray(0); // attribute 0            
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0,                  // attribute 0                    
		3,                  // number of componentes (3 floats)                    
		GL_FLOAT,           // data type                    
		GL_FALSE,           // should be normalized?                   
		0,                  // stride                    
		(void*)0            // array buffer offset                   
	);
	glDrawArrays(GL_TRIANGLES, 0, 3); // start at 0 and 3 tris            
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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