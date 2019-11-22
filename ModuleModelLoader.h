#pragma once
#include "Module.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

using namespace Assimp;
class ModuleModelLoader :
	public Module
{
public:
	ModuleModelLoader();
	~ModuleModelLoader();
	//bool Init();

	

	std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<Mesh> meshes;

	void LogInfo(const std::string& pMessage); 
	void LogError(const std::string& pMessage);

	void LoadModel(const char*);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh*, const aiScene*);
	std::vector<Texture> loadTextures(aiMaterial*, aiTextureType, char*);
	const char* directory = nullptr;
	const char* fullPath = nullptr;
	const char* fullPath2 = nullptr;
	const char* sourcePath = "./";
	const char* myTexturesPath = "./Textures/Baker_house.png";

	

	Mesh theMesh;
	float3 modelPos = {0, 0, 0};


	int numVertices = 0;
	int numIndices = 0;
	int numFaces = 0;
	//bool CleanUp();
};

