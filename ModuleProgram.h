#include "Module.h"
#include "GL/glew.h"
#include "MathGeoLib.h"
class ModuleProgram :
	public Module
{
public:
	ModuleProgram();
	~ModuleProgram();

	bool Init();
	GLuint LoadShader(const char*, const char*);
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

	GLuint VertexShaderID;
	GLuint FragmentShaderID;
	GLuint ProgramID;
	bool CleanUp();
};

