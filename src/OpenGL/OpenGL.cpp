#include "OpenGL.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

OpenGL::OpenGL() : _appWindow(NULL), _isRunning(false)
{
    // _appWindow is instantiated in glfwCreateWindow()
}

OpenGL::~OpenGL()
{
    delete _appWindow;
}

bool OpenGL::Initialize()
{
    bool initialized = true;
    InitGLFW();

    _appWindow = CreateGLFWWindow();
    initialized &= _appWindow != NULL;
    initialized &= CheckGLADInit();

    if (initialized)
    {
        RegisterCallbacks();
    }

    return initialized;
}

void OpenGL::InitGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLFWwindow* OpenGL::CreateGLFWWindow()
{
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGLStudy", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));

    return window;
}

bool OpenGL::CheckGLADInit()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    return true;
}

void OpenGL::RegisterCallbacks()
{
    glfwSetFramebufferSizeCallback(_appWindow, FramebufferSizeCallback);
}

void OpenGL::Start()
{
    _isRunning = true;
    Update();
}

void OpenGL::Update()
{
    while (!glfwWindowShouldClose(_appWindow))
    {
        ProcessInput();

        // rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(_appWindow);
        glfwPollEvents();
    }

    _isRunning = false;
    glfwTerminate();
}

void OpenGL::ProcessInput()
{
    if (glfwGetKey(_appWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(_appWindow, true);
    }
}

void OpenGL::SetViewport(int x, int y, int w, int h)
{
    glViewport(x, y, w, h);
}

void OpenGL::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    if (OpenGL* openGl = reinterpret_cast<OpenGL*> (glfwGetWindowUserPointer(window)))
    {
        openGl->SetViewport(0, 0, width, height);
    }
}
