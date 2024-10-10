#include "Shader.h"

Shader::Shader() :
    pointLightCount(0),
    uniformPointLightCount(0),
    spotLightCount(0),
    uniformSpotLightCount(0),
    shaderID(0), 
    uniformModel(0), 
    uniformProjection(0), 
    uniformView(0),
    uniformTexture(0),
    uniformEyePosition(0),
    uniformSpecularIntensity(0),
    uniformShininess(0),
    uniformDirectionalLightTransform(0),
    uniformDirectionalShadowMap(0),
    uniformOmniLightPos(0),
    uniformFarPlane(0),
    uniformLightMatrices(),
    uniformDirectionalLight(),
    uniformPointLight(),
    uniformSpotLight(),
    uniformOmniShadowMap()
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

void Shader::CreateFromFiles(const char* vertexPath, const char* geometryPath, const char* fragmentPath)
{
    std::string vertexString = ReadFile(vertexPath);
    std::string geometryString = ReadFile(geometryPath);
    std::string fragmentString = ReadFile(fragmentPath);

    const char* vertexCode = vertexString.c_str();
    const char* geometryCode = geometryString.c_str();
    const char* fragmentCode = fragmentString.c_str();

    CompileShader(vertexCode, geometryCode, fragmentCode);
}

void Shader::Validate()
{
    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glValidateProgram(shaderID); // Validates the shader program

    // Checking for any shader validation error
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, 1024, NULL, eLog);
        printf("Error validating program: %s \n", eLog);
    }
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

GLuint Shader::GetOmniLightPosLocation()
{
    return uniformOmniLightPos;
}

GLuint Shader::GetFarPlaneLocation()
{
    return uniformFarPlane;
}

void Shader::SetDirectionalLight(DirectionalLight* dLight)
{
    dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor,
        uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::SetPointLights(PointLight* pLight, unsigned int lightCount, unsigned int initialTextureUnit, unsigned int offset)
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

        pLight[i].GetShadowMap()->Read(GL_TEXTURE0 + initialTextureUnit + i);
        glUniform1i(uniformOmniShadowMap[i + offset].shadowMap, initialTextureUnit + i);
        glUniform1f(uniformOmniShadowMap[i + offset].farPlane, pLight[i].GetFarPlane());
    }
}

void Shader::SetSpotLights(SpotLight* sLight, unsigned int lightCount, unsigned int initialTextureUnit, unsigned int offset)
{
    if (lightCount > MAX_SPOT_LIGHTS)
        lightCount = MAX_SPOT_LIGHTS;

    glUniform1i(uniformSpotLightCount, lightCount);

    for (size_t i = 0; i < lightCount; i++)
    {
        sLight[i].UseLight(
            uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColor,
            uniformSpotLight[i].uniformDiffuseIntensity, 
            uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDirection,
            uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear,
            uniformSpotLight[i].uniformExponent, uniformSpotLight[i].uniformEdge
        );

        sLight[i].GetShadowMap()->Read(GL_TEXTURE0 + initialTextureUnit + i);
        glUniform1i(uniformOmniShadowMap[i + offset].shadowMap, initialTextureUnit + i);
        glUniform1f(uniformOmniShadowMap[i + offset].farPlane, sLight[i].GetFarPlane());
    }
}

void Shader::SetTexture(GLuint textureUnit)
{
    glUniform1i(uniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit)
{
    glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4 lTransform)
{
    glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(lTransform));
}

void Shader::SetOmniLightPos(glm::vec3 lightPos)
{
    glUniform3f(uniformOmniLightPos, lightPos.x, lightPos.y, lightPos.z);
}

void Shader::SetFarPlane(GLfloat farPlane)
{
    glUniform1f(uniformFarPlane, farPlane);
}

void Shader::SetLightMatrices(std::vector<glm::mat4> lightMatrices)
{
    for (size_t i = 0; i < 6; i++)
    {
        glUniformMatrix4fv(uniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
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

void Shader::CompileProgram()
{
    glLinkProgram(shaderID); // Link the shader  program, which will create the "executables" in the GPU

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

    // Point Light
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

    // Spot Light
    uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

    for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
    {
        char locBuff[100] = { '\0' };
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.color", i);
        uniformSpotLight[i].uniformColor = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
        uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
        uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
        uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
        uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
        uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
        uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
        uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
        uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
    }


    uniformTexture = glGetUniformLocation(shaderID, "theTexture");
    uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform");
    uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");

    uniformOmniLightPos = glGetUniformLocation(shaderID, "lightPos");
    uniformFarPlane = glGetUniformLocation(shaderID, "farPlane");

    for (size_t i = 0; i < 6; i++)
    {
        char locBuff[100] = { '\0' };
        snprintf(locBuff, sizeof(locBuff), "lightMatrices[%d]", i);
        uniformLightMatrices[i] = glGetUniformLocation(shaderID, locBuff);
    }

    for (size_t i = 0; i < MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS; i++)
    {
        char locBuff[100] = { '\0' };
        snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].shadowMap", i);
        uniformOmniShadowMap[i].shadowMap = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].farPlane", i);
        uniformOmniShadowMap[i].farPlane = glGetUniformLocation(shaderID, locBuff);
    }
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

    CompileProgram();
}

void Shader::CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode)
{
    shaderID = glCreateProgram(); // Creates an empty shader program and stores its ID in the var shader (GLuint)

    if (!shaderID)
    {
        printf("Error creating shader program! \n");
        return;
    }

    AddShader(shaderID, vertexCode, GL_VERTEX_SHADER); // Creates, compile and attach the VERTEX shader to the shader program
    AddShader(shaderID, geometryCode, GL_GEOMETRY_SHADER); // Creates, compile and attach the GEOMETRY shader to the shader program
    AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER); // Creates, compile and attach the FRAGMENT shader to the shader program

    CompileProgram();
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
