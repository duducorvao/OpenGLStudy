#include "Light.h"

Light::Light() :
	color(glm::vec3(1.0f, 1.0f, 1.0f)),
	ambientIntensity(1.0f),
	diffuseIntensity(0.0f),
	lightProj(),
	shadowMap()
{}

Light::~Light(){} 

Light::Light(GLuint shadowWidth, GLuint shadowHeight,
	GLfloat red, GLfloat green,	GLfloat blue,
	GLfloat aIntensity,	GLfloat dIntensity
) :
	color(glm::vec3(red, green, blue)),
	ambientIntensity(aIntensity),
	diffuseIntensity(dIntensity),
	lightProj(),
	shadowMap()
{
	shadowMap = new ShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight);
}
