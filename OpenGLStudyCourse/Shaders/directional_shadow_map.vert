#version 330

layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 directionalLightTransform; // How we convert pos into the POV of the directional light (projection * camera view);

void main()
{
	// converting pos to world space relative to the light and where it is looking
	gl_Position = directionalLightTransform * model * vec4(pos, 1.0);
}