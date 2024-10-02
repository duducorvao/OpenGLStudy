#include "Model.h"

Model::Model() : 
	meshList(),
	textureList(),
	meshToTex()
{
}

Model::~Model()
{
}

void Model::LoadModel(const std::string& fileName)
{
	Assimp::Importer importer;

	unsigned int flags = aiProcess_Triangulate | aiProcess_FlipUVs | 
		aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices;

	const aiScene* scene = importer.ReadFile(fileName, flags);

	if (!scene)
	{
		printf("Model (%s) failed to load: %s\n", fileName, importer.GetErrorString());
		return;
	}

	LoadNode(scene->mRootNode, scene);
	LoadMaterials(scene);
}

void Model::RenderModel()
{
	for (size_t i = 0; i < meshList.size(); i++)
	{
		unsigned int materialIndex = meshToTex[i]; // meshList and meshToText must have same size;

		if (materialIndex < textureList.size() && textureList[materialIndex])
		{
			textureList[materialIndex]->UseTexture();
		}

		meshList[i]->RenderMesh();
	}
}

void Model::ClearModel()
{
	for (size_t i = 0; i < meshList.size(); i++)
	{
		if (meshList[i])
		{
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}

	for (size_t i = 0; i < textureList.size(); i++)
	{
		if (textureList[i])
		{
			delete textureList[i];
			textureList[i] = nullptr;
		}
	}
}

void Model::LoadNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		// node->mMeshes[i] holds the ID of the mesh
		// scene->mMeshes[] holds the mesh itself, but we need the ID to access it
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		// Recursion to iterate over all the "rootNode" children
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z }); // Vertices pos
		
		if (mesh->HasTextureCoords(0)) // Course code is mesh->mTextureCoords[0]
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y}); // UV
		}
		else
		{
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}

		vertices.insert(vertices.end(), { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z }); // Normals
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++) // indices
	{
		aiFace face = mesh->mFaces[i];

		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* newMesh = new Mesh();
	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	meshList.push_back(newMesh);
	meshToTex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene* scene)
{
	textureList.resize(scene->mNumMaterials);
	
	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];
		textureList[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				std::string pathStr = std::string(path.data);
				int index = pathStr.rfind("\\"); // fixing any absolute path issue
				std::string filename = pathStr.substr(index + 1);

				std::string texPath = std::string("Textures/") + filename;

				textureList[i] = new Texture(texPath.c_str());

				if (!textureList[i]->LoadTexture(GL_RGB))
				{
					printf("Failed to load texture at: %s\n", texPath);
					delete textureList[i];
					textureList[i] = nullptr;
				}
			}
		}

		// Texture error fallback
		if (!textureList[i])
		{
			textureList[i] = new Texture("Textures/missing.png");
			textureList[i]->LoadTexture(GL_RGB);
		}
	}
}
