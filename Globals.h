#pragma once
#include <windows.h>
#include <stdio.h>
#include "SDL.h"
#include "GL/glew.h"
#include "MathGeoLib.h"
#include <vector>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FULLSCREEN false
#define RESIZABLE true
#define VSYNC true
#define TITLE "Super Awesome Engine"

struct Vertex {
	float3 Position;
	float3 Normal;
	float2 TexCoords;
	float3 Tangent;
	float3 Bitangent;
};

struct Texture {
	unsigned int id;
	char* type;
	const char* path;
	int width;
	int height;
	unsigned char* data;
};

struct Mesh {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
};