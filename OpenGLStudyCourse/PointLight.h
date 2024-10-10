#pragma once

#include <vector>
#include "Light.h"
#include "OmniShadowMap.h"

class PointLight : public Light
{
public:
	PointLight();
	~PointLight();
	PointLight(GLuint shadowWidth, GLuint shadowHeight,
		GLfloat near, GLfloat far,
		GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat con, GLfloat lin, GLfloat exp);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
		GLuint diffuseIntensityLocation,GLuint positionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

	std::vector<glm::mat4> CalculateLightTransform();
	GLfloat GetFarPlane();
	glm::vec3 GetPosition();

protected:
	glm::vec3 position;

	// Attenuation values for: ax^2 + bx + c
	GLfloat constant; // c
	GLfloat linear; // b
	GLfloat exponent; // a

	// For omni shadow maps
	GLfloat farPlane;
};

