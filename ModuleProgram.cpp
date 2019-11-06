#include "ModuleProgram.h"
#include "MathGeoLib.h"
#include <iostream>
#include <fstream>
#include <sstream>



ModuleProgram::ModuleProgram()
{
}


ModuleProgram::~ModuleProgram()
{
}

bool ModuleProgram::Init() 
{
	VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	ProgramID = glCreateProgram();
	return true;
}

GLuint ModuleProgram::LoadShader(const char* vertex_file_path, const char* fragment_file_path)
{
	GLint Result = GL_FALSE;
	int InfoLogLengthVs;
	int InfoLogLengthFs;
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertex_file_path);
		fShaderFile.open(fragment_file_path);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	//----------------------------------------Vertex--------------------------------------
	glShaderSource(VertexShaderID, 1, &vShaderCode, NULL);
	glCompileShader(VertexShaderID);
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLengthVs);
	if (InfoLogLengthVs > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLengthVs + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLengthVs, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}
	// Link the program
	printf("Linking program\n");
	glAttachShader(ProgramID, VertexShaderID);
	//glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLengthVs);
	if (InfoLogLengthVs > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLengthVs + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLengthVs, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
	//------------------Fragment-----------------------
	glShaderSource(FragmentShaderID, 1, &fShaderCode, NULL);
	glCompileShader(FragmentShaderID);
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLengthFs);
	if (InfoLogLengthFs > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLengthFs + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLengthFs, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}
	// Link the program
	printf("Linking program\n");
	glAttachShader(ProgramID, FragmentShaderID);
	//glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLengthFs);
	if (InfoLogLengthFs > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLengthFs + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLengthFs, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}



update_status ModuleProgram::PreUpdate()
{

	return UPDATE_CONTINUE;
}

update_status ModuleProgram::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleProgram::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleProgram::CleanUp()
{
	LOG("Destroying program");

	//Destroy window
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
	glDeleteProgram(ProgramID);
	return true;
}