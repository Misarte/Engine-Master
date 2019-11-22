
#include "Module.h"
#include "MathGeoLib.h"
#include "./Geometry/Frustum.h"
#include "GL/glew.h"
#include "./Math/float3.h"
#include "./Math/float4x4.h"

enum Axis
{
	X,
	Y,
	Z,
};

class ModuleCamera :
	public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	void ShowGrid();
	void ShowAxis();

	//void MoveCam(Axis);
	void ZoomIn();
	void ZoomOut();

	Frustum frustum;
	float4x4 LookAt(float3, float3, float3);
	float4x4 proj;
	float4x4 model;
	float4x4 view;
	float aspect = 1.0f;
	float rotSpeed = 0.02f;;
	float speedCamera = SPEED_CAM;
	float distance;

	void RotateCam(const float xAxis, const float yAxis);
	void OrbitCam(const float xAxis, const float yAxis);
	void Focus(const float3& fPos);
	void SetFOV(float fov);
	void SetAspectRatio();
	/*float3 cameraPos = App->camera->frustum.pos;
	float3 cameraUp = math::float3(0.0f, 1.0f, 0.0f);
	float3 cameraFront = math::float3(0.0f, -1.0f, -4.0f);*/

	bool CleanUp();
};

