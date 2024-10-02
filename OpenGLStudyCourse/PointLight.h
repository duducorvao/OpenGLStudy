#pragma once
#include "Light.h"
class PointLight : public Light
{
public:
	PointLight();
	~PointLight();
	PointLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat con, GLfloat lin, GLfloat exp);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
		GLuint diffuseIntensityLocation,GLuint positionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

protected:
	glm::vec3 position;

	// Attenuation values for: ax^2 + bx + c
	GLfloat constant; // c
	GLfloat linear; // b
	GLfloat exponent; // a
};

