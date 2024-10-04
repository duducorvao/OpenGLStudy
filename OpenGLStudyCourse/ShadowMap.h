#pragma once

#include <stdio.h>

#include <GL/glew.h>

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	virtual bool Init(GLuint width, GLuint height);
	virtual void Write();
	virtual void Read(GLenum textureUnit);
	GLuint GetShadowWidth() { return shadowWidth; }
	GLuint GetShadowHeight() { return shadowHeight; }

protected:
	GLuint FBO; // Framebuffer Object;
	GLuint shadowMap;
	GLuint shadowWidth;
	GLuint shadowHeight;
};

