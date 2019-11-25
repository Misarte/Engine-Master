#include "Module.h"
#include "SDL.h"
#include "GL/glew.h"
#include "MathGeoLib.h"

class ModuleTriangle : public Module
{
public:
	ModuleTriangle();
	~ModuleTriangle();
	bool Init();
	void DrawMesh(Mesh mesh);
	void SetUpMesh(Mesh mesh);
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

private:
	GLuint vbo, vao, ebo;
};

