#include "GLWindow.h"

GLWindow::GLWindow() : 
    mainWindow(NULL),
    width(800),
    height(600),
    bufferWidth(0),
    bufferHeight(0),
    lastX(0.0f),
    lastY(0.0f),
    xChange(0.0f),
    yChange(0.0f),
    mouseFirstMoved(true)
{
    InitialiseKeys();
}

GLWindow::GLWindow(GLint windowWidth, GLint windowHeight) : 
    mainWindow(NULL),
    bufferWidth(0),
    bufferHeight(0),
    lastX(0.0f),
    lastY(0.0f),
    xChange(0.0f),
    yChange(0.0f),
    mouseFirstMoved(true)
{
	width = windowWidth;
	height = windowHeight;

    InitialiseKeys();
}

GLWindow::~GLWindow()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}

int GLWindow::Initialise()
{
    // Initialise GLFW
    if (!glfwInit())
    {
        printf("GLFW initialisation failed");
        glfwTerminate();
        return 1;
    }

    // Setup GLFW window properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core profile = no backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mainWindow = glfwCreateWindow(width, height, "OpenGLStudyCourse", NULL, NULL);
    if (!mainWindow)
    {
        printf("GLFW window creation failed.");
        glfwTerminate();
        return 1;
    }

    // Get buffer size information
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // Handle Key + Mouse Input
    CreateCallbacks();
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initialisation failed.");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST);

    // Setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    // Input
    glfwSetWindowUserPointer(mainWindow, this);
}

GLfloat GLWindow::GetXChange()
{
    GLfloat change = xChange;
    xChange = 0.0f;
    return change;
}

GLfloat GLWindow::GetYChange()
{
    GLfloat change = yChange;
    yChange = 0.0f;
    return change;
}

void GLWindow::InitialiseKeys()
{
    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = false;
    }
}

void GLWindow::CreateCallbacks()
{
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
    
}

void GLWindow::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
    GLWindow* glWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            glWindow->keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            glWindow->keys[key] = false;
        }
    }
}

void GLWindow::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
    GLWindow* glWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));

    if (glWindow->mouseFirstMoved)
    {
        glWindow->lastX = xPos;
        glWindow->lastY = yPos;
        glWindow->mouseFirstMoved = false;
    }

    glWindow->xChange = xPos - glWindow->lastX;
    glWindow->yChange = glWindow->lastY - yPos;

    glWindow->lastX = xPos;
    glWindow->lastY = yPos;
}
