#include "ModuleModelLoader.h"
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleInput.h"
#include "ModuleIMGUI.h"
#include "ModuleCamera.h"
#include <sys/stat.h>
#include <io.h>
#include <fstream>
#include <iostream>
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>
#include <assimp/DefaultLogger.hpp>

class myStream :
	public LogStream
{
public:
	// Constructor
	myStream()
	{
		// empty
	}

	// Destructor
	~myStream()
	{
		// empty
	}
	// Write womethink using your own functionality
	void write(const char* message)
	{
		App->imgui->AddLog("ASSIMP INFO %s\n", message);
	}
};
ModuleModelLoader::ModuleModelLoader()
{
}

ModuleModelLoader::~ModuleModelLoader()
{
}
void ModuleModelLoader::LogInfo(const std::string& pMessage)
{
	Assimp::DefaultLogger::get()->info(pMessage);
	//App->imgui->AddLog(Assimp::DefaultLogger::get()->info(pMessage));

}

void ModuleModelLoader::LogError(const std::string& pMessage)
{
	Assimp::DefaultLogger::get()->error(pMessage);
}

void ModuleModelLoader::LoadModel(const char* path)
{
	meshes.clear();
	// Create a logger instance 
	DefaultLogger::create("", Logger::VERBOSE);
	// Now I am ready for logging my stuff
	DefaultLogger::get()->info("this is my info-call");
	// Kill it after the work is done
	const unsigned int severity = Logger::Debugging | Logger::Info | Logger::Err | Logger::Warn;
	Assimp::Importer importer;
	LogInfo("importer.ReadFile");
	Assimp::DefaultLogger::get()->attachStream(new myStream(), severity);
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	if (scene)
	{
		LogInfo("Import done");
	}
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		App->imgui->AddLog("ERROR::ASSIMP:: ", importer.GetErrorString());
		LogError("Error parsing file");
		return;
	}
	directory = path;
	processNode(scene->mRootNode, scene);
	DefaultLogger::kill();
}

void ModuleModelLoader::processNode(aiNode* node, const aiScene* scene)
{
	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
	App->camera->Focus();
}

Mesh ModuleModelLoader::processMesh(aiMesh* mesh, const aiScene* scene)
{
	Mesh loadedMesh;
	boundingBox = { float3::zero, float3::zero };
	// Walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		numVertices = mesh->mNumVertices;
		//mesh.
		Vertex vertex;
		float3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		if (vector.x > boundingBox.maxPoint.x)
		{
			boundingBox.maxPoint.x = vector.x;
		}
		if ((vector.y > vector.x) & (vector.y > boundingBox.maxPoint.y))
		{
			boundingBox.maxPoint.y = vector.y;
		}
		if ((vector.z > vector.x) & (vector.z > vector.z) & (vector.z > boundingBox.maxPoint.z))
		{
			boundingBox.maxPoint.z = vector.z;
		}
		//===========,==================================================================================
		if (vector.x < boundingBox.minPoint.x)
		{
			boundingBox.minPoint.x = vector.x;
		}
		if ((vector.y < vector.x) & (vector.y < boundingBox.minPoint.y))
		{
			boundingBox.minPoint.y = vector.y;
		}
		if ((vector.z < vector.x) & (vector.z < vector.z) & (vector.z < boundingBox.minPoint.z))
		{
			boundingBox.minPoint.z = vector.z;
		}
		//===========,==================================================================================
		
		//boundingBox.Enclose(boundingBox., mesh->mNumVertices);
		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			float2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = float2(0.0f, 0.0f);
		// tangent
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;
		// bitangent
		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.Bitangent = vector;
		loadedMesh.vertices.push_back(vertex);
		//modelPos = vector - maxRadius;
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		numFaces = mesh->mNumFaces;
		aiFace face = mesh->mFaces[i];
		numIndices = face.mNumIndices;
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			loadedMesh.indices.push_back(face.mIndices[j]);
	}
	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	// 1. diffuse maps
	std::vector<Texture> diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	loadedMesh.textures.insert(loadedMesh.textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	std::vector<Texture> specularMaps = loadTextures(material, aiTextureType_SPECULAR, "texture_specular");
	loadedMesh.textures.insert(loadedMesh.textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = loadTextures(material, aiTextureType_HEIGHT, "texture_normal");
	loadedMesh.textures.insert(loadedMesh.textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = loadTextures(material, aiTextureType_AMBIENT, "texture_height");
	loadedMesh.textures.insert(loadedMesh.textures.end(), heightMaps.begin(), heightMaps.end());

	// return a mesh object created from the extracted mesh data
	return loadedMesh;
}

std::vector<Texture> ModuleModelLoader::loadTextures(aiMaterial* mat, aiTextureType type, char* typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		App->texture->CleanUp();
		aiString str;
		mat->GetTexture(type, i, &str);
		App->imgui->AddLog("Texture Name that should be loaded: %s\n", str.C_Str());
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		 // if texture hasn't been loaded already, load it
		if (FileExists(str.C_Str()))
		{
			App->imgui->AddLog("Texture loading from: %s\n", str.C_Str());
			finalPath = str.C_Str();
		}
		else if (FileExists(modelPath.c_str()))
		{
			modelPath.append(str.C_Str());
			App->imgui->AddLog("Texture loading from Models Path: %s\n", modelPath.c_str());
			finalPath = modelPath.c_str();
		}
		else
		{
			myTexturesPath.append(str.C_Str());
			if (FileExists(myTexturesPath.c_str()))
			{
				App->imgui->AddLog("Texture loading from Textures Path: %s\n", myTexturesPath.c_str());
				finalPath = myTexturesPath.c_str();
			}
		}
		Texture texture;
		texture = App->texture->LoadTexture(finalPath.c_str());
		texture.type = typeName;
		textures.push_back(texture);
	}
	return textures;
}
void ModuleModelLoader::UpdateTexture(Texture& textToUpdate)
{
	for (int i = 0; i < App->model->meshes.size(); i++)
	{
		meshes[i].textures.clear();
		meshes[i].textures.push_back(textToUpdate);
	}
}

bool ModuleModelLoader::FileExists(const char* path)
{
	if (access(path, 0) == -1)
	{
		App->imgui->AddLog("File does NOT exist in path %s:\n", path);
		return false;
	}
	return true;
}








