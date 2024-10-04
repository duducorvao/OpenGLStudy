#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "GLWindow.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Model.h"

#define ArraySize(x) sizeof(x) / sizeof(x[0])

const float toRadians = 3.14159265f / 180.0f;

GLWindow mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;
Shader directionalShadowShader;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

Material shinyMaterial;
Material dullMaterial;

Model plant;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/phong.vert";

// Fragment Shader
static const char* fShader = "Shaders/phong.frag";

// Uniforms
GLuint uniformProjection = 0, uniformModel = 0,
uniformView = 0, uniformEyePosition = 0,
uniformSpecularIntensity = 0, uniformShininess = 0;

void CalcAverageNormals(unsigned int* indices, unsigned int indiceCount,
    GLfloat* vertices, unsigned int verticesCount,
    unsigned int vLength, unsigned int normalOffset)
{

    for (size_t i = 0; i < indiceCount; i += 3)
    {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;

        glm::vec3 v1(
            vertices[in1] - vertices[in0],
            vertices[in1 + 1] - vertices[in0 + 1],
            vertices[in1 + 2] - vertices[in0 + 2]);

        glm::vec3 v2(
            vertices[in2] - vertices[in0],
            vertices[in2 + 1] - vertices[in0 + 1],
            vertices[in2 + 2] - vertices[in0 + 2]);

        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

        in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;

        vertices[in0] += normal.x;
        vertices[in0 + 1] += normal.y;
        vertices[in0 + 2] += normal.z;

        vertices[in1 ] += normal.x;
        vertices[in1 + 1] += normal.y;
        vertices[in1 + 2] += normal.z;

        vertices[in2] += normal.x;
        vertices[in2 + 1] += normal.y;
        vertices[in2 + 2] += normal.z;
    }

    // We're doing this because of the "vertices[in0] +=" above. Since we're incrementing (+=)
    // the previously normalized normals could get "un-normalized". So we normalize "again" in
    // the loop below.
    for (size_t i = 0; i < verticesCount / vLength; i++) // Each row of vertices[] array below
    {
        unsigned int nOffset = i * vLength + normalOffset;

        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);

        vertices[nOffset] = vec.x;
        vertices[nOffset + 1] = vec.y;
        vertices[nOffset + 2] = vec.z;
    }
}

void CreateObjects()
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
     //  X      Y     Z     U     V     NX    NY    NZ
       -1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, -1.0f,  1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, -0.6f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f,  1.0f,  0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f,
    };

    unsigned int floorIndices[] = {
        0, 2, 1,
        1, 2, 3
    };

    GLfloat floorVertices[] = {
       -10.0f, 0.0f, -10.0f,  0.0f,  0.0f,  0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, -10.0f,  10.0f, 0.0f,  0.0f, -1.0f, 0.0f,
       -10.0f, 0.0f,  10.0f,  0.0f,  10.0f, 0.0f, -1.0f, 0.0f,
        10.0f, 0.0f,  10.0f,  10.0f, 10.0f, 0.0f, -1.0f, 0.0f
    };

    CalcAverageNormals(indices, ArraySize(indices), vertices, ArraySize(vertices), 8, 5);

    Mesh* triangle = new Mesh();
    triangle->CreateMesh(vertices, indices, ArraySize(vertices), ArraySize(indices));
    meshList.push_back(triangle);
    meshList.push_back(triangle);

    Mesh* floor = new Mesh();
    floor->CreateMesh(floorVertices, floorIndices, ArraySize(floorVertices), ArraySize(floorIndices));
    meshList.push_back(floor);
}

void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(shader1);

    directionalShadowShader = Shader();
    directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");
}

void RenderScene()
{
    // Triangle 1
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));

    // Updates the uniform with id "uniformModel" with the value "model"
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    brickTexture.UseTexture();
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[0]->RenderMesh();

    // Triangle 2
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.5f));

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    dirtTexture.UseTexture();
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[1]->RenderMesh();

    // Floor 1
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    dirtTexture.UseTexture();
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[2]->RenderMesh();

    // Plant
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.1f, 0.1, 0.1f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    plant.RenderModel();
}

void DirectionalShadowMapPass(DirectionalLight* light)
{
    directionalShadowShader.UseShader();

    glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());

    light->GetShadowMap()->Write();
    glClear(GL_DEPTH_BUFFER_BIT);

    uniformModel = directionalShadowShader.GetModelLocation();
    directionalShadowShader.SetDirectionalLightTransform(light->CalculateLightTransform());

    RenderScene();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
    shaderList[0]->UseShader();

    uniformModel = shaderList[0]->GetModelLocation();
    uniformProjection = shaderList[0]->GetProjectionLocation();
    uniformView = shaderList[0]->GetViewLocation();
    uniformEyePosition = shaderList[0]->GetEyePositionLocation();
    uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();
    uniformShininess = shaderList[0]->GetShininessLocation();

    glViewport(0, 0, 1366, 768); // Maybe create a "SetViewPort" in Window class to handle this better;

    // Clear window
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Updates the uniform with id "uniformProjection" with the value "projection"
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniform3f(uniformEyePosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);

    // Passing values to shaders' uniforms
    shaderList[0]->SetDirectionalLight(&mainLight);
    shaderList[0]->SetPointLights(pointLights, pointLightCount);
    shaderList[0]->SetSpotLights(spotLights, spotLightCount);
    shaderList[0]->SetDirectionalLightTransform(mainLight.CalculateLightTransform());

    // Bind our shadow map to GL_TEXTURE1 since GL_TEXTURE0 is bound to the mesh's texture.
    mainLight.GetShadowMap()->Read(GL_TEXTURE1);
    shaderList[0]->SetTexture(0);
    shaderList[0]->SetDirectionalShadowMap(1);

    spotLights[0].SetFlash(camera.GetCameraPosition(), camera.GetCameraDirection());

    RenderScene();
}

int main()
{
    mainWindow = GLWindow(1366, 768);
    mainWindow.Initialise();

    CreateObjects();
    CreateShaders();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, -90.0f, 2.0f, 0.1f);

    brickTexture = Texture("Textures/brick.png");
    brickTexture.LoadTexture(GL_RGBA);

    dirtTexture = Texture("Textures/dirt.png");
    dirtTexture.LoadTexture(GL_RGBA);

    plainTexture = Texture("Textures/plain.png");
    plainTexture.LoadTexture(GL_RGBA);

    shinyMaterial = Material(4.0f, 256);
    dullMaterial = Material(0.3f, 4);

    plant = Model();
    plant.LoadModel("Models/indoor plant_02.obj");

    mainLight = DirectionalLight(
        1024, 1024,
        1.0f, 1.0f, 1.0f,
        0.1f, 0.3f,
        0.0f, 5.0f, 5.0f);

    pointLights[0] = PointLight(
        0.0f, 0.0f, 1.0f,
        0.0f, 0.8f,
        0.0f, 0.0f, 0.0f,
        0.3f, 0.2f, 0.1f);

    pointLightCount++;

    pointLights[1] = PointLight(
        0.0f, 1.0f, 0.0f,
        0.0f, 0.8f,
        -4.0f, 2.0f, 0.0f,
        0.3f, 0.1f, 0.1f);

    pointLightCount++;

    spotLights[0] = SpotLight(
        1.0f, 1.0f, 1.0f,
        0.0f, 2.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.1f,
        20.0f);

    spotLightCount++;

    spotLights[1] = SpotLight(
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, -1.5f, 0.0f,
        -100.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        20.0f);

    spotLightCount++;

    glm::mat4 projection = glm::perspective(45.0f, mainWindow.GetBufferWidth() / mainWindow.GetBufferHeight(), 0.1f, 100.0f);

    // Loop until window closed
    while (!mainWindow.GetShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        // Get and handle user input events
        glfwPollEvents();

        camera.KeyControl(mainWindow.GetKeys(), deltaTime);
        camera.MouseControl(mainWindow.GetXChange(), mainWindow.GetYChange());

        // Render the depth buffer to a texture using the main light
        DirectionalShadowMapPass(&mainLight);
        RenderPass(projection, camera.CalculateViewMatrix());
        
        glUseProgram(0); // Unbind the shader

        mainWindow.SwapBuffers();
    }

    return 0;
}
