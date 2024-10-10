#version 330

layout (triangles) in;
// triangle_strip always calculates a triangle when we add a new vertex, using the last 2 vertices.
layout (triangle_strip, max_vertices=18) out;

uniform mat4 lightMatrices[6]; // Each side of a cubemap

out vec4 FragPos;

void main() 
{
    for(int face = 0; face < 6; face++)
    {
        gl_Layer = face; // Each gl_Layer is a texture of the cubemap that we created in OmniShadowMap.cpp
        
        for(int i = 0; i < 3; i++)
        {
            FragPos = gl_in[i].gl_Position; // gl_in is the variable in line 3

            // Set the triangle position (gl_Position) to the fragment position in relation to the lightMatrices[face] (view matrix)
            gl_Position = lightMatrices[face] * FragPos;

            // Emits a vertex at position Gl_Position to the layer gl_Layer
            EmitVertex();
        }
        EndPrimitive();
    }
}