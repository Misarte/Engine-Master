#pragma once
#include "Module.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>
#include <assimp/DefaultLogger.hpp>

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
	void UpdateTexture(Texture& textToUpdate);
	bool FileExists(const char*);
	const char* directory = nullptr;
	std::string modelPath = "Models/";
	std::string myTexturesPath = "Textures/";
	std::string finalPath = "";

	

	Mesh theMesh;
	float3 modelPos = {0, 0, 0};
	float maxRadius = 7.0f;
	AABB boundingBox = {float3::zero, float3::zero};
	float3 centerPoint = { 0, 0, 0 };
	float3 centerOffset = { 0, 0, 0 };
	int numVertices = 0;
	int numIndices = 0;
	int numFaces = 0;
	//bool CleanUp();
};

