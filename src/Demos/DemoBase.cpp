#include "DemoBase.h"

DemoBase::DemoBase() : m_OpenGL(nullptr) {}

bool DemoBase::Init()
{
	m_OpenGL = new OpenGL();
	m_OpenGL->Initialize();

	return m_OpenGL->m_isInitialized;
}

void DemoBase::InitBuffers() {}

void DemoBase::InitShaders() {}

void DemoBase::Start()
{
	m_OpenGL->Start();
    Tick();
}

void DemoBase::Release()
{
    // m_OpenGL is deleted inside glfwTerminate();
    m_OpenGL = nullptr;
}

void DemoBase::Tick()
{
    while (!glfwWindowShouldClose(MAIN_WINDOW))
    {
        Update();
        PreDraw();
        Draw();
        PostDraw();
    }

    m_OpenGL->Release();
    Release();
}

void DemoBase::Update()
{
    ProcessInput();
}

void DemoBase::PreDraw()
{
    // Clearing stuff
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void DemoBase::Draw() {}

void DemoBase::PostDraw()
{
    glfwSwapBuffers(MAIN_WINDOW);
    glfwPollEvents();
}

void DemoBase::ProcessInput()
{
    if (glfwGetKey(MAIN_WINDOW, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(MAIN_WINDOW, true);
    }
}