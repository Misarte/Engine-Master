
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
	bool CleanUp();
};

