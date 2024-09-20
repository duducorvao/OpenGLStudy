#include "DemoHelloTriangle.h"

DemoHelloTriangle::DemoHelloTriangle() : 
	m_vertices{ 
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f },
	 m_VAO {0},
	 m_VBO {0},
	 m_vertexShader {0},
	 m_fragmentShader {0},
	 m_shaderProgram {0}
{
}

bool DemoHelloTriangle::Init()
{
	if (DemoBase::Init()) {
		m_OpenGL->SetViewport(0, 0, 800, 600);
		m_OpenGL->Start();
	}

	InitBuffers();
	InitShaders();

	return m_OpenGL->m_isInitialized;
}

void DemoHelloTriangle::ProcessInput()
{
	DemoBase::ProcessInput();
}

void DemoHelloTriangle::Draw()
{
	// draw our first triangle
	glUseProgram(m_shaderProgram);
	glBindVertexArray(m_VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void DemoHelloTriangle::Release()
{
	DemoBase::Release();
}

void DemoHelloTriangle::InitBuffers()
{
	// Vertex Array Object
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Vertex Buffer Object
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	// Copy our "triangle" vertex data to the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

	// Set vertex attributes pointers (like a descriptor)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Note that this is allowed. The call to glVertexAttribPointer above registered the VBO as the 
	// vertex attribute's bound VBO so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DemoHelloTriangle::InitShaders()
{
	// Vertex Shader
	m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_vertexShader, 1, &m_vertexShaderSource, NULL);
	glCompileShader(m_vertexShader);

	int  vertexSuccess;
	char infoLog[512];
	glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &vertexSuccess);

	if (!vertexSuccess)
	{
		glGetShaderInfoLog(m_vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment Shader
	m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_fragmentShader, 1, &m_fragmentShaderSource, NULL);
	glCompileShader(m_fragmentShader);

	int  fragmentSuccess;
	char infoLogFrag[512];
	glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &fragmentSuccess);

	if (!fragmentSuccess)
	{
		glGetShaderInfoLog(m_fragmentShader, 512, NULL, infoLogFrag);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLogFrag << std::endl;
	}

	if (vertexSuccess && fragmentSuccess)
	{
		m_shaderProgram = glCreateProgram();

		glAttachShader(m_shaderProgram, m_vertexShader);
		glAttachShader(m_shaderProgram, m_fragmentShader);
		glLinkProgram(m_shaderProgram);

		int shaderProgramSucess;
		char infoLogShader[512];

		glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &shaderProgramSucess);
		if (!shaderProgramSucess)
		{
			glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLogShader);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLogShader << std::endl;
			return;
		}

		glUseProgram(m_shaderProgram);
		glDeleteShader(m_vertexShader);
		glDeleteShader(m_fragmentShader);
	}
}