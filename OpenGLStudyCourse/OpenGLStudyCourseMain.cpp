#include <stdio.h>
#include <string.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

GLuint VAO, VBO, IBO, shader, uniformModel ;

float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.005f;

float currAngle = 0.0f;

float currSize = 0.4f;
float currIncrement = 0.001f;
float maxSize = 0.8f;
float minSize = 0.1f;

// Vertex Shader
static const char* vShader = "                                        \n\
#version 330                                                          \n\
                                                                      \n\
layout (location = 0) in vec3 pos;                                    \n\
                                                                      \n\
out vec4 vCol;                                                        \n\
                                                                      \n\
uniform mat4 model;                                                   \n\
                                                                      \n\
void main()                                                           \n\
{                                                                     \n\
    gl_Position = model * vec4(pos, 1.0);                             \n\
    vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);                        \n\
}";

// Fragment Shader
static const char* fShader = "                    \n\
#version 330                                      \n\
                                                  \n\
in vec4 vCol;                                     \n\
                                                  \n\
out vec4 color;                                   \n\
void main()                                       \n\
{                                                 \n\
    color = vCol;                                 \n\
}";

void CreateTriangle()
{
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    // A triangle
    GLfloat vertices[] =
    {
       -1.0f, -1.0f, 0.0f,  // Vertex 0
        0.0f, -1.0f, 1.0f,  // Vertex 1
        1.0f, -1.0f, 0.0f,  // Vertex 2
        0.0f,  1.0f, 0.0f   // Vertex 3
    };

    glGenVertexArrays(1, &VAO); // Create one Vertex Array Object and store its ID in var VAO
    glBindVertexArray(VAO); // Bind this VAO to the current OpenGL's context

        glGenBuffers(1, &IBO); // Create one Index Buffer Object and store its ID in var IBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // Bind this IBO as a GL_ELEMENT_ARRAY_BUFFER
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Fill the IBO with the indexes data

        glGenBuffers(1, &VBO); // Create one Vertex Buffer Object and store its ID in var VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind this VBO to the context (which will automatically bind it to the bound VAO)
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Fill the VBO with the vertices data

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Describe how this VBO's attributes are organized
            glEnableVertexAttribArray(0); // Enable the attribute at index 0 (which in this case is position)

        glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO

    glBindVertexArray(0); // Unbind the VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbinding IBO after VAO
}

void AddShader(GLuint program, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType); // Create an empty shader and stores its ID in the var theShader

    const GLchar* theCode[1];
    theCode[0] = shaderCode; // Stores the hardcoded shaderCode (const char*) here

    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode); // Store the shader code length which will be needed later on

    glShaderSource(theShader, 1, theCode, codeLength); // Specify that "theCode" will be the "theShader" code to be used 
    glCompileShader(theShader); // Compiles the shader

    // Checking for any shader compilation error
    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, 1024, NULL, eLog);
        printf("Error compiling the %d shader: %s \n", shaderType, eLog);
        return;
    }

    glAttachShader(program, theShader); // Attach the compiled shader to the program
}

void CompileShaders()
{
    shader = glCreateProgram(); // Creates an empty shader program and stores its ID in the var shader (GLuint)

    if (!shader)
    {
        printf("Error creating shader program! \n");
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER); // Creates, compile and attach the VERTEX shader to the shader program
    AddShader(shader, fShader, GL_FRAGMENT_SHADER); // Creates, compile and attach the FRAGMENT shader to the shader program

    glLinkProgram(shader); // Link the shader program, which will create the "executables" in the GPU

    // Checking for any shader compilation error
    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, 1024, NULL, eLog);
        printf("Error linking program: %s \n", eLog);
        return;
    }

    glValidateProgram(shader); // Validates the shader program

    // Checking for any shader validation error
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, 1024, NULL, eLog);
        printf("Error validating program: %s \n", eLog);
        return;
    }

    uniformModel = glGetUniformLocation(shader, "model");
}

int main()
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

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "OpenGLStudyCourse", NULL, NULL);
    if (!mainWindow)
    {
        printf("GLFW window creation failed.");
        glfwTerminate();
        return 1;
    }

    // Get buffer size information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

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

    CreateTriangle();
    CompileShaders();

    // Loop until window closed
    while (!glfwWindowShouldClose(mainWindow))
    {
        // Get and handle user input events
        glfwPollEvents();

        triOffset += triIncrement;

        if (abs(triOffset) >= triMaxOffset)
        {
            triIncrement *= -1.0f;
        }

        currAngle += 0.1f;
        if (currAngle >= 360)
        {
            currAngle -= 360;
        }

        currSize += currIncrement;

        if (currSize >= maxSize || currSize <= minSize)
        {
            currIncrement *= -1.0f;
        }

        // Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader); // Bind our shader program
        
            glm::mat4 model = glm::mat4(1.0f);
            //model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
            model = glm::rotate(model, currAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

            // Updates the uniform with id "uniformModel" with the value "model"
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); 

            glBindVertexArray(VAO); // Bind the VAO we want OpenGL to render
            
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // Bind IBO
                glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

            glBindVertexArray(0); // Unbind the VAO
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind IBO

        glUseProgram(0); // Unbind the shader

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}
