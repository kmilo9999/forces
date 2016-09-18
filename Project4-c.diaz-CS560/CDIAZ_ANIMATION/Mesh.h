/* Start Header *****************************************************************/
/*!
\file Mesh.h
\author Camilo Diaz
\e-mail c.diaz@digipen.edu
\date 9/24/2015
\Brief Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or
disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
*/
/* End Header *******************************************************************/

#pragma once
#include <GL/glew.h>
#include "Math\Vector3.h"
#include "Math\Vector2.h"
#include "Math\Matrix4.h"
#include "Texture.h"
#include <vector>
#include "Math\Transformation.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;
class Mesh
{
public:
	Mesh();
	~Mesh();
	Mesh(vector<Vector3>& vertices, vector<GLuint>& indices);
	Mesh(vector<Vector3>& vertices, vector<GLuint>& indices, vector<Vector2>& textcoords, vector<Texture*>& textures);
	bool LoadFromFle(string fileName);
	void Render();
	Transformation transformation;
	Matrix4 FinalTransformation;
	bool HasTexture();
	Mesh* Clone();
	void AddTexture(Texture* texture);
	vector<Texture*>& GetTextures();

private:
	GLuint VAO, VBO, Tbuff, EBO;
	void setupMesh();
	void setupArrays(vector<Vector3>& vertices, vector<GLuint>& indices, vector<Vector2>& textcoords, vector<Texture*>& textures);
	vector<Vector3> vertices;
	vector<Vector2> texturecoordinates;
	vector<GLuint> indices;
	vector<Texture*> textures;
	void ProcessNode(aiNode* node, const aiScene* scene);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);

};

