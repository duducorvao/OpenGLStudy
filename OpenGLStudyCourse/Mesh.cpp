#include "Mesh.h"

Mesh::Mesh() : VAO(0), VBO(0), IBO(0), indexCount(0) {}

Mesh::~Mesh()
{
    ClearMesh();
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	indexCount = numOfIndices;

    glGenVertexArrays(1, &VAO); // Create one Vertex Array Object and store its ID in var VAO
    glBindVertexArray(VAO); // Bind this VAO to the current OpenGL's context

        glGenBuffers(1, &IBO); // Create one Index Buffer Object and store its ID in var IBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // Bind this IBO as a GL_ELEMENT_ARRAY_BUFFER
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW); // Fill the IBO with the indexes data

        glGenBuffers(1, &VBO); // Create one Vertex Buffer Object and store its ID in var VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind this VBO to the context (which will automatically bind it to the bound VAO)
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW); // Fill the VBO with the vertices data

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Describe how this VBO's attributes are organized
        glEnableVertexAttribArray(0); // Enable the attribute at index 0 (which in this case is position)

        glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO

    glBindVertexArray(0); // Unbind the VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbinding IBO after VAO
}

void Mesh::RenderMesh()
{
    if (VAO == 0 || VBO == 0 || IBO == 0) return;

    glBindVertexArray(VAO); // Bind the VAO we want OpenGL to render

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // Bind IBO
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0); // Unbind the VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind IBO
}

void Mesh::ClearMesh()
{
    if (IBO != 0)
    {
        glDeleteBuffers(1, &IBO);
        IBO = 0;
    }

    if (VBO != 0)
    {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }

    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
        IBO = 0;
    }

    indexCount = 0;
}
