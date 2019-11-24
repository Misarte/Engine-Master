#include "ModuleCamera.h"
#include "ModuleTriangle.h"
#include "MathGeoLib.h"
#include "ModuleInput.h"
#include "ModuleIMGUI.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleModelLoader.h"
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
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3(0.0f, 0.0f, 4.0f);
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	SetAspectRatio();
	RecalculateRot(frustum.front);
	return true;
}

update_status ModuleCamera::PreUpdate()
{
	SetAspectRatio();
	proj = frustum.ProjectionMatrix();

	model = float4x4::FromTRS(float3(0.0f, 0.0f, -4.0f),
			float3x3::RotateY(math::pi / 4.0f),
		    float3(1.0f, 1.0f, 1.0f));

	view = LookAt(frustum.pos, frustum.front + frustum.pos, frustum.up);
	return UPDATE_CONTINUE;
}
update_status ModuleCamera::Update()
{
	yaw = 0.0f;
	pitch = 0.0f;
	speedCamera = SPEED_CAM;
	if ((App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) || (App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT))
	{
		App->imgui->AddLogInput("Input: Left/Right Shift Keyboard\n");
		speedCamera = speedCamera * 2;
	}
	//While Right clicking, “WASD” fps-like movement and free look around must be enabled.
	if (App->input->GetMouseButtonDown(SDL_BUTTON(SDL_BUTTON_RIGHT)) & SDL_BUTTON(3))
	{
		//RotateCam();
		App->imgui->AddLogInput("Input: Right Mouse\n");
		float2 move = App->input->GetMouseMotion();
		yaw = move.x;
		pitch = move.y;

		RotateCam(yaw * rotSpeed, pitch * rotSpeed);
		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT)
		{
			App->imgui->AddLogInput("Input: Q Keyboard\n");
			frustum.pos += speedCamera * frustum.up;
		}
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT)
		{
			frustum.pos -= speedCamera * frustum.up;
			App->imgui->AddLogInput("Input: E Keyboard\n");
		}
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			frustum.pos += speedCamera * (frustum.front.Cross(frustum.up)).Normalized();
			App->imgui->AddLogInput("Input: D Keyboard\n");
		}
		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			frustum.pos -= speedCamera * (frustum.front.Cross(frustum.up)).Normalized();
			App->imgui->AddLogInput("Input: A Keyboard\n");
		}
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		{
			frustum.pos += speedCamera * frustum.front;
			App->imgui->AddLogInput("Input: W Keyboard\n");
		}
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		{
			frustum.pos -= speedCamera * frustum.front;
			App->imgui->AddLogInput("Input: S Keyboard\n");
		}
	}
	else
	{
		RotateCam(0.0f, 0.0f);
	}
	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT & App->input->GetMouseButtonDown(SDL_BUTTON(SDL_BUTTON_LEFT)))
	{
		App->imgui->AddLogInput("Input: Left ALT Keyboard & Left Mouse\n");
		float2 move = App->input->GetMouseMotion();
		yaw = move.x;
		pitch = move.y;

		OrbitCam(yaw * rotSpeed, pitch * rotSpeed);
		

		/*float3 dir;
		dir.x = cos(DegToRad(yaw)) * cos(DegToRad(pitch));
		dir.y = sin(DegToRad(pitch));
		dir.z = sin(DegToRad(pitch)) * cos(DegToRad(pitch));*/
		
	}
	if (App->input->GetKey(SDL_SCANCODE_F))
	{
		App->imgui->AddLogInput("Input: F Keyboard\n");
		Focus();
	}
	return UPDATE_CONTINUE;
}
void ModuleCamera::ZoomIn()
{
	App->imgui->AddLogInput("Input: Scroll Up Mouse Wheel\n");
	frustum.pos += speedCamera * frustum.front;
}

void ModuleCamera::ZoomOut()
{
	App->imgui->AddLogInput("Input: Scroll Down Mouse Wheel\n");
	frustum.pos -= speedCamera * frustum.front;
}

void ModuleCamera::RotateCam(const float xAxis, const float yAxis)
{/*
	yaw += move.x;
	pitch += move.y;

	float3 dir;
	dir.x = cos(DegToRad(yaw)) * cos(DegToRad(pitch));
	dir.y = sin(DegToRad(pitch));
	dir.z = sin(DegToRad(yaw)) * cos(DegToRad(pitch));

	frustum.front = dir.Normalized();*/
	if (xAxis!= 0.0f)
	{
		float3x3 rotY = float3x3::RotateY(xAxis);
		frustum.front = rotY.Transform(frustum.front).Normalized();
		frustum.up = rotY.Transform(frustum.up).Normalized();
	}
	if (yAxis != 0.0f)
	{
		float3x3 rotX = float3x3::RotateAxisAngle(frustum.WorldRight(), yAxis);
		frustum.up = rotX.Transform(frustum.up).Normalized();
		frustum.front = rotX.Transform(frustum.front).Normalized();
	}
}
void ModuleCamera::OrbitCam(const float xAxis, const float yAxis)
{
	if (xAxis != 0.0f)
	{
		float3x3 rot = float3x3::RotateY(xAxis);
		frustum.pos = rot.Transform(frustum.pos - App->model->centerPoint) + App->model->centerPoint;
		//frustum.front = rot.Transform(frustum.front).Normalized();
		
	}
	if (yAxis != 0.0f)
	{
		float3x3 rot = float3x3::RotateX(yAxis);
		frustum.pos = rot.Transform(frustum.pos - App->model->centerPoint) + App->model->centerPoint;
		//frustum.front = rot.Transform(frustum.front).Normalized();
	}
	LookAt(frustum.pos, App->model->centerPoint, frustum.up);
	/*proj = frustum.ProjectionMatrix();
	view = frustum.ViewMatrix();*/
}

void ModuleCamera::SetFOV(float fov)
{
	frustum.verticalFov = fov;
	SetAspectRatio();
	proj = frustum.ProjectionMatrix();
}
void ModuleCamera::SetAspectRatio()
{
	frustum.horizontalFov = 2.0f * atanf(tanf(frustum.verticalFov * 0.5f) * ((float)App->window->width / App->window->height));
}

void ModuleCamera::Focus()
{
	frustum.pos = App->model->boundingBox.CenterPoint() - App->model->boundingBox.Size().Normalize() * frustum.front;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	App->camera->view = frustum.ViewMatrix();
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

void ModuleCamera::RecalculateRot(float3& vec)
{
	pitch = RadToDeg(asin(vec.y));
	yaw = RadToDeg(acos(vec.x / cos(DegToRad(pitch))));
}
