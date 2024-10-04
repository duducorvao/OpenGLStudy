#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() :
	Light(),
	direction(glm::vec3(0.0f, -1.0f, 0.0f))
{
	lightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
}

DirectionalLight::~DirectionalLight() {}

DirectionalLight::DirectionalLight(
	GLuint shadowWidth, GLuint shadowHeight,
	GLfloat red, GLfloat green,	GLfloat blue,
	GLfloat aIntensity,	GLfloat dIntensity,
	GLfloat xDir, GLfloat yDir,	GLfloat zDir
) :
	Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity),
	direction(glm::vec3(xDir, yDir, zDir))
{
	lightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f); 
}

void DirectionalLight::UseLight(
	GLuint ambientIntensityLocation,
	GLuint ambientColorLocation,
	GLuint diffuseIntensityLocation,
	GLuint directionLocation)
{
	glUniform3f(ambientColorLocation, color.r, color.g, color.b);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	// The light transform is basically the projection and the view combined;
	return lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f),	glm::vec3(0.0f, 1.0f, 0.0f));
}