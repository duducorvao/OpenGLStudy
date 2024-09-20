#include "OpenGL.h"


OpenGL::OpenGL() : m_appWindow(NULL), m_isInitialized(false), m_isRunning(false)
{
    // _appWindow is instantiated in glfwCreateWindow()
}

OpenGL::~OpenGL()
{
    delete m_appWindow;
}

bool OpenGL::Initialize()
{
    bool initialized = true;
    InitGLFW();

    m_appWindow = CreateGLFWWindow();
    initialized &= m_appWindow != NULL;
    initialized &= CheckGLADInit();

    if (initialized)
    {
        RegisterCallbacks();
    }

    m_isInitialized = initialized;

    return m_isInitialized;
}

void OpenGL::Release()
{
    m_isRunning = false;
    m_isInitialized = false;
    glfwTerminate();
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
        Release();
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
    glfwSetFramebufferSizeCallback(m_appWindow, FramebufferSizeCallback);
}

void OpenGL::Start()
{
    m_isRunning = true;
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
