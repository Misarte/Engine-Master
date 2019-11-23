
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
	float rotSpeed = 0.02f;
	float rotOffset = 0.0f;
	float speedCamera = SPEED_CAM;
	float distance;
	float dz = 2.0f;
	float3 centerOffset = {0, 0, 0};
	float offset = 3.0f ;
	void RotateCam();
	void OrbitCam(const float xAxis, const float yAxis);
	void Focus();
	void SetFOV(float fov);
	void SetAspectRatio();
	void RecalculateRot(float3& vec);
	float yaw = 0.0f, pitch = 0.0f;
	/*float3 cameraPos = App->camera->frustum.pos;
	float3 cameraUp = math::float3(0.0f, 1.0f, 0.0f);
	float3 cameraFront = math::float3(0.0f, -1.0f, -4.0f);*/

	bool CleanUp();
};

