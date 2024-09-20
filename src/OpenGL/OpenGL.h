#pragma once

//struct GLFWwindow;
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class OpenGL
{

public:
	GLFWwindow* m_appWindow;
	bool m_isInitialized;
	bool m_isRunning;

	OpenGL();
	~OpenGL();

	// Inits

	bool Initialize();
	void Release();
	void InitGLFW();
	GLFWwindow* CreateGLFWWindow();
	bool CheckGLADInit();
	void RegisterCallbacks();
	void Start();

	// Configs

	/// <summary>
	/// calls glViewport and set position and size of the viewport
	/// </summary>
	/// <param name="x">x location of lower left corner</param>
	/// <param name="y">y location of lower left corner</param>
	/// <param name="w">Width</param>
	/// <param name="h">Height</param>
	void SetViewport(int x, int y, int w, int h);

	// Callbacks
	
	/// <summary>
	/// Called everytime the window is resized.
	/// </summary>
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
};

