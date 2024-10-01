#include "Shader.h"

Shader::Shader() :
    pointLightCount(0),
    uniformPointLightCount(0),
    shaderID(0), 
    uniformModel(0), 
    uniformProjection(0), 
    uniformView(0),
    uniformEyePosition(0),
    uniformSpecularIntensity(0),
    uniformShininess(0)
{}

Shader::~Shader()
{
    ClearShader();
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexString = ReadFile(vertexPath);
    std::string fragmentString = ReadFile(fragmentPath);

    const char* vertexCode = vertexString.c_str();
    const char* fragmentCode = fragmentString.c_str();

    CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char* filePath)
{
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open())
    {
        printf("File to read %s! File doesn't exist.", filePath);
        return "";
    }

    std::ostringstream oss;
    oss << fileStream.rdbuf();

    return oss.str();
}

GLuint Shader::GetProjectionLocation()
{
    return uniformProjection;
}

GLuint Shader::GetModelLocation()
{
    return uniformModel;
}

GLuint Shader::GetViewLocation()
{
    return uniformView;
}

GLuint Shader::GetAmbientIntensityLocation()
{
    return uniformDirectionalLight.uniformAmbientIntensity;
}

GLuint Shader::GetAmbientColorLocation()
{
    return uniformDirectionalLight.uniformColor;
}

GLuint Shader::GetDiffuseIntensityLocation()
{
    return uniformDirectionalLight.uniformDiffuseIntensity;
}

GLuint Shader::GetDirectionLocation()
{
    return uniformDirectionalLight.uniformDirection;
}

GLuint Shader::GetSpecularIntensityLocation()
{
    return uniformSpecularIntensity;
}

GLuint Shader::GetShininessLocation()
{
    return uniformShininess;
}

GLuint Shader::GetEyePositionLocation()
{
    return uniformEyePosition;
}

void Shader::SetDirectionalLight(DirectionalLight* dLight)
{
    dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor,
        uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}
void Shader::SetPointLights(PointLight* pLight, unsigned int lightCount)
{
    if (lightCount > MAX_POINT_LIGHTS)
        lightCount = MAX_POINT_LIGHTS;

    glUniform1i(uniformPointLightCount, lightCount);

    for (size_t i = 0; i < lightCount; i++)
    {
        pLight[i].UseLight(
            uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColor,
            uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
            uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear,
            uniformPointLight[i].uniformExponent
        );
    }
}

void Shader::UseShader()
{
    glUseProgram(shaderID);
}

void Shader::ClearShader()
{
    if (shaderID != 0)
    {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }

    uniformModel = 0;
    uniformProjection = 0;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
    shaderID = glCreateProgram(); // Creates an empty shader program and stores its ID in the var shader (GLuint)

    if (!shaderID)
    {
        printf("Error creating shader program! \n");
        return;
    }

    AddShader(shaderID, vertexCode, GL_VERTEX_SHADER); // Creates, compile and attach the VERTEX shader to the shader program
    AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER); // Creates, compile and attach the FRAGMENT shader to the shader program

    glLinkProgram(shaderID); // Link the shader program, which will create the "executables" in the GPU

    // Checking for any shader compilation error
    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, 1024, NULL, eLog);
        printf("Error linking program: %s \n", eLog);
        return;
    }

    glValidateProgram(shaderID); // Validates the shader program

    // Checking for any shader validation error
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, 1024, NULL, eLog);
        printf("Error validating program: %s \n", eLog);
        return;
    }

    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformProjection = glGetUniformLocation(shaderID, "projection");
    uniformView = glGetUniformLocation(shaderID, "view");
    uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderID, "directionalLight.base.color");
    uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
    uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
    uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
    uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");

    uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

    for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        char locBuff[100] = { '\0' };
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
        uniformPointLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
        uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
        uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
        uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
        uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
        uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
        uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
    }
}

void Shader::AddShader(GLuint program, const char* shaderCode, GLenum shaderType)
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
