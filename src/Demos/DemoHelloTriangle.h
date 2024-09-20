#pragma once
#include "DemoBase.h"

class DemoHelloTriangle : public DemoBase
{

private:
	float m_vertices[9];
	unsigned int m_VAO;
	unsigned int m_VBO;

	const char* m_vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	unsigned int m_vertexShader;

	const char* m_fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";
	unsigned int m_fragmentShader;

	unsigned int m_shaderProgram;

public:

	DemoHelloTriangle();

	// Inherited via DemoBase
	bool Init() override;
	void ProcessInput() override;
	void Draw() override;
	void Release() override;
	void InitBuffers() override;
	void InitShaders() override;
};