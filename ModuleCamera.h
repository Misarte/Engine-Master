
#include "Module.h"
#include "MathGeoLib.h"
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


	Frustum frustum;
	float4x4 LookAt(float3, float3, float3);
	float4x4 proj;
	float4x4 model;
	float4x4 view;
	float speedCamera = 5.0f;
	float3 cameraPos = math::float3(0.0f, 1.0f, 4.0f);
	float3 cameraUp = math::float3(0.0f, 1.0f, 0.0f);
	float3 cameraFront = math::float3(0.0f, -1.0f, -4.0f);

	bool CleanUp();
};

