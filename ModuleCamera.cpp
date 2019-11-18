#include "ModuleCamera.h"
#include "ModuleTriangle.h"
#include "MathGeoLib.h"
#include "ModuleInput.h"
#include "ModuleProgram.h"
#include "Globals.h"
#include "Application.h"
#include "GL/glew.h"
#include "MathGeoLib.h"
#include "IL/il.h"



ModuleCamera::ModuleCamera()
{
}


ModuleCamera::~ModuleCamera()
{
}

float4x4 ModuleCamera::LookAt(float3 eye, float3 target, float3 up) {
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
bool ModuleCamera::Init()
{
	float aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3(0.0f, 0.0f, 4.0f);
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect);
	

	float4x4 transform = proj * view * float4x4(model);

	float4 v1 = transform * float4(-1.0f, -1.0f, 0.0f, 1.0f);
	float4 v2 = transform * float4(1.0f, -1.0f, 0.0f, 1.0f);
	float4 v3 = transform * float4(0.0f, 1.0f, 0.0f, 1.0f);

	v1 = v1 / v1.w;
	v2 = v2 / v2.w;
	v3 = v3 / v3.w;
	
	return true;
}

update_status ModuleCamera::PreUpdate()
{
	proj = frustum.ProjectionMatrix();

	model = float4x4::FromTRS(float3(0.0f, 0.0f, -4.0f),
			float3x3::RotateY(math::pi / 4.0f),
		    float3(1.0f, 1.0f, 1.0f));

	view = LookAt(frustum.pos, frustum.front + frustum.pos, frustum.up);
	return UPDATE_CONTINUE;
}
update_status ModuleCamera::Update()
{
	//While Right clicking, “WASD” fps-like movement and free look around must be enabled.
	//if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT))
	//{
		LOG("MOUSE RIGHT CLICK\n");
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			App->camera->frustum.pos.x += App->camera->speedCamera;
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			App->camera->frustum.pos.x -= App->camera->speedCamera;
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			App->camera->frustum.pos.y += App->camera->speedCamera;
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			App->camera->frustum.pos.y -= App->camera->speedCamera;
		}
	//}

	//Mouse wheel should zoom in and out (move close the camera, do not use FOV to zoom)
	if (App->input->GetMouseButtonDown(SDL_MOUSEWHEEL_NORMAL) == KEY_DOWN)
	{
		frustum.pos.z += speedCamera;
	}
	if (App->input->GetMouseButtonDown(SDL_MOUSEWHEEL_FLIPPED) == KEY_DOWN)
	{
		frustum.pos.z -= speedCamera;
	}
	
	return UPDATE_CONTINUE;
}

void ModuleCamera::ShowGrid()
{
	glLineWidth(1.0f);
	float d = 200.0f;
	glBegin(GL_LINES);
	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}
	glEnd();
}

void ModuleCamera::ShowAxis()
{
	glBegin(GL_LINES);
	// red X 
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f); 
	glVertex3f(0.0f, 0.0f, 0.0f); 
	glVertex3f(1.0f, 0.0f, 0.0f); 
	glVertex3f(1.0f, 0.1f, 0.0f); 
	glVertex3f(1.1f, -0.1f, 0.0f); 
	glVertex3f(1.1f, 0.1f, 0.0f); 
	glVertex3f(1.0f, -0.1f, 0.0f);
	// green Y 
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f); 
	glVertex3f(0.0f, 0.0f, 0.0f); 
	glVertex3f(0.0f, 1.0f, 0.0f); 
	glVertex3f(-0.05f, 1.25f, 0.0f); 
	glVertex3f(0.0f, 1.15f, 0.0f); 
	glVertex3f(0.05f, 1.25f, 0.0f); 
	glVertex3f(0.0f, 1.15f, 0.0f); 
	glVertex3f(0.0f, 1.15f, 0.0f); 
	glVertex3f(0.0f, 1.05f, 0.0f);
	// blue Z 
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f); 
	glVertex3f(0.0f, 0.0f, 0.0f); 
	glVertex3f(0.0f, 0.0f, 1.0f); 
	glVertex3f(-0.05f, 0.1f, 1.05f); 
	glVertex3f(0.05f, 0.1f, 1.05f); 
	glVertex3f(0.05f, 0.1f, 1.05f); 
	glVertex3f(-0.05f, -0.1f, 1.05f); 
	glVertex3f(-0.05f, -0.1f, 1.05f); 
	glVertex3f(0.05f, -0.1f, 1.05f);
	glEnd();
}
update_status ModuleCamera::PostUpdate()
{

	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	return true;
}
