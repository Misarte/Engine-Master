#include "ModuleModelLoader.h"
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleIMGUI.h"
#include <sys/stat.h>
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
}

Mesh ModuleModelLoader::processMesh(aiMesh* mesh, const aiScene* scene)
{
	Mesh loadedMesh;
	// Walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		float3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
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
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
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
		aiString str;
		mat->GetTexture(type, i, &str);
		App->imgui->AddLog("Texture Name that should be loaded: %s\n", str.C_Str());
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		bool skipToSourceFolder = false;
		bool skipToMyFolder = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path, str.C_Str()) == 0)
			{
				App->imgui->AddLog("Texture was loaded from fbx description path: %s\n", textures_loaded[j].path);
				textures.push_back(textures_loaded[j]);
				//myTexturesPath = str.C_Str().append("Textures/");
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture = App->texture->LoadTexture(str.C_Str());
			App->imgui->AddLog("Seeking Texture in fbx description path: %s\n", str.C_Str());
			std::fstream file("Baker_house.png");
			if (!file)
			{
				App->imgui->AddLog("File does NOT exist in fbx description path\n");
				skipToSourceFolder = true;
			}
			if (file)
			{
				App->imgui->AddLog("Found File in fbx description path\n");
			}
			//texture.path = str.C_Str();
			texture.type = typeName;
			textures.push_back(texture);
			textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			break;
		}
		//if (skipToSourceFolder)
		//{   // if texture hasn't been loaded already, load it
		//	Texture texture;
		//	
		//	fullPath =+ sourcePath;
		//	fullPath =+"Baker_house.png";
		//	App->imgui->AddLog(fullPath);
		//	std::fstream file(fullPath);
		//	if (!file)
		//	{
		//		App->imgui->AddLog("File does NOT exist in source folder path\n");
		//		skipToMyFolder = true;
		//	}
		//	texture = App->texture->LoadTexture(fullPath);
		//	App->imgui->AddLog("Seeking Texture source folder path: %s\n", fullPath);
		//	//texture.path = str.C_Str();
		//	texture.type = typeName;
		//	textures.push_back(texture);
		//	textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		//	break;
		//}
		//if (skipToMyFolder)
		//{   // if texture hasn't been loaded already, load it
		//	Texture texture;
		//	fullPath2 =+ myTexturesPath;
		//	fullPath2 =+"Baker_house.png";
		//	App->imgui->AddLog(fullPath2);
		//	std::fstream file(fullPath2);
		//	if (file)
		//	{
		//		App->imgui->AddLog("Found File in my Textures folder path\n");
		//	}
		//	texture = App->texture->LoadTexture(myTexturesPath);
		//	App->imgui->AddLog("Seeking Texture in my Textures path: %s\n", fullPath2);
		//	
		//	//texture.path = str.C_Str();
		//	texture.type = typeName;
		//	textures.push_back(texture);
		//	textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		//}
	}
	return textures;
}






