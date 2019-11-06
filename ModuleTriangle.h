#include "Module.h"
#include "SDL.h"
#include "GL/glew.h"
#include "MathGeoLib.h"

class ModuleTriangle : public Module
{
public:
	ModuleTriangle();
	~ModuleTriangle();
	float4x4 LookAt(float3, float3, float3);
	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	float4x4 proj;
	float4x4 model;
	float4x4 view;

	bool CleanUp();

private:
	GLuint vbo;
};

