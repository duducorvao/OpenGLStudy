#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	~Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
		GLfloat xDirection, GLfloat yDirection, GLfloat zDirection, GLfloat dIntensity);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
		GLuint diffuseIntensityLocation, GLuint directionLocation);

private:
	glm::vec3 color;
	GLfloat ambientIntensity;

	// Directional Light
	glm::vec3 direction;
	GLfloat diffuseIntensity;
};

