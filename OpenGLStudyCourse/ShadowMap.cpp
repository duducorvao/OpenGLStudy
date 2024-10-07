#include "ShadowMap.h"

ShadowMap::ShadowMap() :
	FBO(0),
	shadowMap(0),
	shadowWidth(0),
	shadowHeight(0)
{ }

ShadowMap::~ShadowMap()
{
	if (FBO)
	{
		glDeleteFramebuffers(1, &FBO);
	}

	if (shadowMap)
	{
		glDeleteTextures(1, &shadowMap);
	}
}

bool ShadowMap::Init(GLuint width, GLuint height)
{
	shadowWidth = width;
	shadowHeight = height;

	glGenFramebuffers(1, &FBO);
	glGenTextures(1, &shadowMap);

	glBindTexture(GL_TEXTURE_2D, shadowMap);

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		shadowWidth, shadowHeight, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, nullptr); // This just initialises the text, not render it.

	// Wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); // X axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); // Y axis
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Describes how to render the framebuffer to the texture (shadowMap)
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer error: %i\n", status);
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind
		
	return true;
}

void ShadowMap::Write()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void ShadowMap::Read(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
}
