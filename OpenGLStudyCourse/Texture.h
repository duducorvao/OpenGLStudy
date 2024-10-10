#pragma once

#include "CommonValues.h"
#include <GL/glew.h>

class Texture
{
public:
	Texture();
	Texture(const char* fileLoc);

	bool LoadTexture(int glewTextureFormat); // GL_RGB, GL_RGBA
	bool LoadTextureA();
	void UseTexture();
	void ClearTexture();

	~Texture();

private:
	GLuint textureID;
	int width;
	int height;
	int bitDepth;

	const char* fileLocation;
};

