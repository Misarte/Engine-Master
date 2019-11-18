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
	
	void LoadModel(const char*);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh*, const aiScene*);
	const char* directory = nullptr;
	const char* fullPath = nullptr;
	const char* fullPath2 = nullptr;
	const char* sourcePath = "./";
	const char* myTexturesPath = "./Textures/Baker_house.png";

	std::vector<Texture> loadTextures(aiMaterial* , aiTextureType, char*);
	//bool CleanUp();
};

