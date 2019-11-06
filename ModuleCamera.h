#pragma once
#include "Module.h"
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


	//Frustum frustum;

	bool CleanUp();
};

