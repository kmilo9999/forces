/* Start Header *****************************************************************/
/*!
\file Mesh.cpp
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 9/24/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/

#include "Mesh.h"


Mesh::Mesh() :transformation()
{
}


Mesh::~Mesh()
{
}

Mesh::Mesh(vector<Vector3>& vertices, vector<GLuint>& indices) :transformation()
{
	setupArrays(vertices, indices, vector<Vector2>(), vector<Texture*>());
}

Mesh::Mesh(vector<Vector3>& vertices, vector<GLuint>& indices, vector<Vector2>& textcoords, vector<Texture*>& textures) : transformation()
{
	setupArrays(vertices, indices, textcoords, textures);
}

void Mesh::setupArrays(vector<Vector3>& vertices, vector<GLuint>& indices, vector<Vector2>& ntextcoords, vector<Texture*>& ntextures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->texturecoordinates = ntextcoords;
	this->textures = ntextures;
	this->setupMesh();
}

Mesh* Mesh::Clone()
{
	 return  new Mesh(this->vertices, this->indices, this->texturecoordinates, this->textures);
}

bool Mesh::LoadFromFle(string fileName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName,
		aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene)
	{
		printf("Mesh %s load failed\n", fileName);
		return false;
	}
	this->ProcessNode(scene->mRootNode, scene);
	return true;
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vector3),
		&this->vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3),
		(GLvoid*)0);

	if (!this->texturecoordinates.empty())
	{
		glGenBuffers(1, &this->Tbuff);
		glBindBuffer(GL_ARRAY_BUFFER, this->Tbuff);
		glBufferData(GL_ARRAY_BUFFER, this->texturecoordinates.size() * sizeof(Vector2),
			&this->texturecoordinates[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2),
			(GLvoid*)0);
	}
	
	glGenBuffers(1, &this->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(int),
		&this->indices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glBindVertexArray(0);
}

void Mesh::Render()
{
	
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::ProcessNode(aiNode* node, const aiScene* scene)
{
	// Process each mesh located at the current node
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		// The node object only contains indices to index the actual objects in the scene. 
		// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->ProcessMesh(mesh, scene);
	}
	// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene);
	}

}

void Mesh::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	// Data to fill
	vector<Vector3> vertices;
	vector<Vector2> uv;
	vector<GLuint> indices;
	vector<Texture*> textures;

	// Walk through each of the mesh's vertices
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		
		Vector3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// Positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		
		vertices.push_back(vector);

		// Texture Coordinates
		Vector2 vec;
		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{

			// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;

		}
		else{
			vec.x = 0.0f;
			vec.y = 0.0f;
		}
			

		uv.push_back(vec);
	}
	// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// Retrieve all indices of the face and store them in the indices vector
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// Process materials
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		aiString Path;

		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			std::string path(Path.data);
			Texture* texture = new Texture(GL_TEXTURE_2D, path.c_str());
			texture->type = "diffuse";
			if (!texture->Load())
			{
				printf("Error loading texture '%s'\n", path.c_str());
				delete texture;
				texture = NULL;
			}
			else
			{
				printf("%d - loaded texture \n");
				textures.push_back(texture);
			}
		}
	}

	// Return a mesh object created from the extracted mesh data
	setupArrays(vertices, indices, uv, textures);
}

bool Mesh::HasTexture()
{
	return textures.size() > 0;
}

void Mesh::AddTexture(Texture* texture)
{
	textures.push_back(texture);
}

vector<Texture*>& Mesh::GetTextures()
{
	return textures;
}