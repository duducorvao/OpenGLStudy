#include "Light.h"

Light::Light() : 
	color(glm::vec3(1.0f, 1.0f, 1.0f)),
	ambientIntensity(1.0f),
	diffuseIntensity(0.0f)
{}

Light::Light(glm::vec3 inColor, GLfloat aIntensity, GLfloat dIntensity) : 
	color(inColor),
	ambientIntensity(aIntensity),
	diffuseIntensity(dIntensity)
{}

Light::~Light(){} 

Light::Light(
	GLfloat red,
	GLfloat green,
	GLfloat blue,
	GLfloat aIntensity,
	GLfloat dIntensity
) :
	color(glm::vec3(red, green, blue)),
	ambientIntensity(aIntensity),
	diffuseIntensity(dIntensity)
{}

//void Light::UseLight(
//	GLuint ambientIntensityLocation,
//	GLuint ambientColorLocation,
//	GLuint diffuseIntensityLocation)
//{
//	glUniform3f(ambientColorLocation, color.r, color.g, color.b);
//	glUniform1f(ambientIntensityLocation, ambientIntensity);
//
//	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
//}
