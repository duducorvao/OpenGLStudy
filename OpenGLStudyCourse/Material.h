#pragma once

#include <GL/glew.h>

class Material
{
public:
	Material();
	~Material();
	Material(GLfloat sIntensity, GLfloat sShine);

	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

private:
	GLfloat specularIntensity; // "Reflectiveness"
	GLfloat shininess; // How "smooth" the surface is
};

