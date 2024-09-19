#pragma once

#include <memory>

struct GLFWwindow;

class OpenGL
{

private:
	GLFWwindow* _appWindow;
	bool _isRunning;

public:
	OpenGL();
	~OpenGL();

	// Inits

	bool Initialize();
	void InitGLFW();
	GLFWwindow* CreateGLFWWindow();
	bool CheckGLADInit();
	void RegisterCallbacks();
	void Start();

	// Updates
	void Update();
	void ProcessInput();

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

