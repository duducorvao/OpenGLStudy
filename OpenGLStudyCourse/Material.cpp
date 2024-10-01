#include "Material.h"

Material::Material() : 
	specularIntensity(0),
	shininess(0)
{}

Material::~Material() {}

Material::Material(GLfloat sIntensity, GLfloat sShine) :
	specularIntensity(sIntensity),
	shininess(sShine)
{}

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation)
{
	glUniform1f(specularIntensityLocation, specularIntensity);
	glUniform1f(shininessLocation, shininess);
}

