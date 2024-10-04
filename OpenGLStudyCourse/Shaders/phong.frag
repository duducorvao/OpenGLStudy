#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 DirectionalLightSpacePos;

out vec4 color;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight 
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float edge;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D theTexture; // GL_TEXTURE0
uniform sampler2D directionalShadowMap; // GL_TEXTURE1
uniform Material material;
uniform vec3 eyePosition; // Camera Position

float CalcDirectionalShadowFactor(DirectionalLight light)
{
	vec3 projCoords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w; // Normalized device coordinates (-1 1)
	projCoords = (projCoords * 0.5) + 0.5; // Normalizing between 0 - 1
	
	float closestDepth = texture(directionalShadowMap, projCoords.xy).r; // Closest position to the light position (at the moment)
	float currentDepth = projCoords.z;
	
	// 0 is close. 1 is far.                  shadow : no shadow
	float shadow = currentDepth < closestDepth ? 1.0 : 0.0;
	
	return shadow;
}

vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor)
{
	vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;
	
	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
	vec4 diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;
	
	vec4 specularColor = vec4(0, 0, 0, 0);
	
	if(diffuseFactor > 0.0f)
	{
		vec3 fragToEye = normalize(eyePosition - FragPos);
		vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));
		float specularFactor = dot(fragToEye, reflectedVertex);
		
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
		}
	}
	
    return (ambientColor + (1.0 - shadowFactor) * (diffuseColor + specularColor));
}

vec4 CalcDirectionalLight()
{
	float shadowFactor = CalcDirectionalShadowFactor(directionalLight);
	return CalcLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}

vec4 CalcPointLight(PointLight pLight)
{
	vec3 direction = FragPos - pLight.position;
		float distance = length(direction);
		direction = normalize(direction);
		
		vec4 color = CalcLightByDirection(pLight.base, direction, 0.0f);
		
		// Quadratic equation
		float attenuation = pLight.exponent * distance * distance + 
							pLight.linear * distance +
							pLight.constant;
		
		return (color/attenuation);
}

vec4 CalcSpotLight(SpotLight sLight)
{
	vec3 rayDirection = normalize(FragPos - sLight.base.position);
	float slFactor = dot(rayDirection, sLight.direction); // angleToFragment
	
	if(slFactor > sLight.edge)
	{
		vec4 color = CalcPointLight(sLight.base);
		//                                                       0.939692616
		return color * (1.0f - (1.0f - slFactor) * (1.0f/(1.0f - sLight.edge)));
	}
	else
	{
		return vec4(0, 0, 0, 0);
	}
}

vec4 CalcPointLights()
{
	vec4 totalColor = vec4(0, 0, 0, 0);
	for(int i = 0; i < pointLightCount; i++)
	{
		totalColor += CalcPointLight(pointLights[i]);
	}
	
	return totalColor;
}

vec4 CalcSpotLights()
{
	vec4 totalColor = vec4(0, 0, 0, 0);
	for(int i = 0; i < spotLightCount; i++)
	{
		totalColor += CalcSpotLight(spotLights[i]);
	}
	
	return totalColor;
}

void main()
{
	vec4 finalColor = CalcDirectionalLight();
	finalColor += CalcPointLights();
	finalColor += CalcSpotLights();
    color = texture(theTexture, TexCoord) * finalColor;
	
	// float shadowFactor = CalcDirectionalShadowFactor(directionalLight);
	// color = vec4(shadowFactor, shadowFactor, shadowFactor, 1.0f);

	// vec3 projCoords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w; // Normalized device coordinates (-1 1)
	// projCoords = (projCoords * 0.5) + 0.5; // Normalizing between 0 - 1
	
	// vec4 shadowMap = texture(directionalShadowMap, projCoords.xy); // Closest position to the light position (at the moment)
	// float camDistMap = projCoords.z;

	// color = vec4(1.0 - shadowMap.xyz, 1.0);
	// //color = vec4(vec3(camDistMap), 1.0);

	// vec3 projCoords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w;
	// color = vec4(vec3(projCoords), 1.0f);
}